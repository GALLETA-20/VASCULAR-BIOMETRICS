#include "GPIO.hpp"
#include "CAMERA.hpp"
#include "PREPROCESAR.hpp"

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/features2d.hpp>
#include <sqlite3.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

#define GPIOCHIP "/dev/gpiochip0"
#define PIN_IR 17
#define CONSUMER "LED4_IR"
#define UMBRAL_MATCHES 25
#define DB_PATH "/home/sallqa/data_base/usuarios.db"

struct Usuario {
    std::string codigo;
    std::string nombre;
    std::string template_path;
};

int contar_matches(const cv::Mat& desc, const cv::Mat& desc_ref)
{
    if(desc.empty() || desc_ref.rows < 2)
        return 0;

    cv::BFMatcher matcher(cv::NORM_HAMMING);
    std::vector<std::vector<cv::DMatch>> knn_matches;
    matcher.knnMatch(desc, desc_ref, knn_matches, 2);

    int buenos = 0;
    for(auto& m : knn_matches)
    {
        if(m.size() == 2 && m[0].distance < 0.75f * m[1].distance)
            buenos++;
    }
    return buenos;
}

int main()
{
    try
    {
        gpioSet LED_IR(GPIOCHIP, PIN_IR, CONSUMER);

        // --- Traer todos los usuarios registrados ---
        sqlite3* db;
        if(sqlite3_open(DB_PATH, &db) != SQLITE_OK)
        {
            std::cerr << "No se pudo abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return -1;
        }

        std::vector<Usuario> usuarios;
        const char* sql = "SELECT codigo, nombre, template_path FROM usuarios;";
        sqlite3_stmt* stmt;
        if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        {
            std::cerr << "Error preparando SELECT: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return -1;
        }

        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            Usuario u;
            u.codigo        = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            u.nombre        = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            u.template_path = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            usuarios.push_back(u);
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);

        if(usuarios.empty())
        {
            std::cerr << "No hay usuarios registrados en la base de datos." << std::endl;
            return -1;
        }

        // --- Captura nueva ---
        LED_IR.ON();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        cv::Mat palma = cap_cam::cap_cam_ir();
        LED_IR.OFF();

        if(palma.empty())
        {
            std::cerr << "Captura fallida" << std::endl;
            return -1;
        }
        if(palma.channels() == 3)
            cv::cvtColor(palma, palma, cv::COLOR_BGR2GRAY);

        cv::Mat prep = preprocesar(palma);
        cv::imwrite("verificacion_captura.png", prep);

        auto orb = cv::ORB::create(500);
        std::vector<cv::KeyPoint> kp;
        cv::Mat desc;
        orb->detectAndCompute(prep, cv::noArray(), kp, desc);

        std::cout << "Keypoints en captura: " << kp.size() << std::endl;

        // --- Comparar contra el template de cada usuario ---
        std::string mejor_codigo, mejor_nombre;
        int mejor_matches = 0;

        for(const auto& u : usuarios)
        {
            cv::Mat desc_ref;
            cv::FileStorage fs(u.template_path, cv::FileStorage::READ);
            if(!fs.isOpened())
            {
                std::cerr << "  (aviso) no se pudo abrir " << u.template_path << std::endl;
                continue;
            }
            fs["descriptores"] >> desc_ref;
            fs.release();

            int buenos = contar_matches(desc, desc_ref);
            std::cout << "  vs " << u.nombre << " (" << u.codigo << "): "
                       << buenos << " matches" << std::endl;

            if(buenos > mejor_matches)
            {
                mejor_matches = buenos;
                mejor_codigo  = u.codigo;
                mejor_nombre  = u.nombre;
            }
        }

        std::cout << "\nMejor coincidencia: " << mejor_matches << " matches" << std::endl;

        if(mejor_matches >= UMBRAL_MATCHES)
        {
            std::cout << "\n>>> ACCESO CONCEDIDO <<<" << std::endl;
            std::cout << "Nombre: " << mejor_nombre << std::endl;
            std::cout << "Codigo: " << mejor_codigo << std::endl;
        }
        else
        {
            std::cout << "\n>>> ACCESO DENEGADO: mano no registrada <<<" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
