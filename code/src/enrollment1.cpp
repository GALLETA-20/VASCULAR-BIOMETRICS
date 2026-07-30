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

#define GPIOCHIP "/dev/gpiochip0"
#define PIN_IR 17
#define CONSUMER "LED4_IR"
#define N_CAPTURAS 5
#define DATA_DIR "/home/sallqa/data_base/"
#define DB_PATH  "/home/sallqa/data_base/usuarios.db"

int main(int argc, char** argv)
{
    if(argc < 3)
    {
        std::cerr << "Uso: " << argv[0] << " <NOMBRE> <CODIGO>" << std::endl;
        return -1;
    }
    std::string nombre = argv[1];
    std::string codigo = argv[2];

    try
    {
        gpioSet LED_IR(GPIOCHIP, PIN_IR, CONSUMER);
        auto orb = cv::ORB::create(500);

        cv::Mat desc_acumulados;

        for(int i = 0; i < N_CAPTURAS; i++)
        {
            std::cout << "Captura " << (i+1) << "/" << N_CAPTURAS
                       << " - poné la mano y presioná ENTER" << std::endl;
            std::cin.get();

            LED_IR.ON();
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            cv::Mat palma = cap_cam::cap_cam_ir();
            LED_IR.OFF();

            if(palma.empty())
            {
                std::cout << "Captura fallida, reintentando..." << std::endl;
                i--;
                continue;
            }
            if(palma.channels() == 3)
                cv::cvtColor(palma, palma, cv::COLOR_BGR2GRAY);

            cv::Mat prep = preprocesar(palma);

            std::vector<cv::KeyPoint> kp;
            cv::Mat desc;
            orb->detectAndCompute(prep, cv::noArray(), kp, desc);

            std::cout << "  Keypoints encontrados: " << kp.size() << std::endl;

            if(desc_acumulados.empty())
                desc_acumulados = desc;
            else
                cv::vconcat(desc_acumulados, desc, desc_acumulados);
        }

        // --- Guardar el .yml con el codigo como nombre de archivo ---
        std::string template_path = std::string(DATA_DIR) + codigo + ".yml";
        cv::FileStorage fs(template_path, cv::FileStorage::WRITE);
        if(!fs.isOpened())
        {
            std::cerr << "No se pudo crear " << template_path << std::endl;
            return -1;
        }
        fs << "descriptores" << desc_acumulados;
        fs.release();

        std::cout << "\nTemplate guardado en " << template_path
                  << " (" << desc_acumulados.rows << " descriptores totales)" << std::endl;

        // --- Insertar en la base de datos ---
        sqlite3* db;
        if(sqlite3_open(DB_PATH, &db) != SQLITE_OK)
        {
            std::cerr << "No se pudo abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return -1;
        }

        const char* sql = "INSERT INTO usuarios (codigo, nombre, template_path) VALUES (?, ?, ?);";
        sqlite3_stmt* stmt;
        if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        {
            std::cerr << "Error preparando INSERT: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return -1;
        }

        sqlite3_bind_text(stmt, 1, codigo.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, nombre.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, template_path.c_str(), -1, SQLITE_TRANSIENT);

        int rc = sqlite3_step(stmt);
        if(rc != SQLITE_DONE)
        {
            // El PRIMARY KEY de "codigo" evita duplicados
            std::cerr << "Error insertando en usuarios (¿codigo ya existe?): "
                      << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return -1;
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        std::cout << "Usuario '" << nombre << "' (codigo " << codigo
                  << ") registrado en la base de datos." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
