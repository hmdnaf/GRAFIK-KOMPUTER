#define _USE_MATH_DEFINES
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>
#include <chrono>

const double gravitasi = 4.0;
const double jari_bola = 0.08;
const double konstanta_parabola = 0.5;
const double redaman = 0.9995;

double posisi_x = -2.0;
double kecepatan = 3.0;
double delta_waktu = 0.0;

double hitungY(double nilai_x) {
    return konstanta_parabola * nilai_x * nilai_x;
}

double hitungPercepatan(double nilai_x, double nilai_v) {
    double turunan_y = 2 * konstanta_parabola * nilai_x;
    double sudut_kemiringan = atan(turunan_y);
    double percepatan_x = -gravitasi * sin(sudut_kemiringan);
    return percepatan_x;
}

void gambarBola(double posisi_x, double posisi_y) {
    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 30; i++) {
        double sudut = i * 2 * M_PI / 30;
        glVertex2f(
            posisi_x + jari_bola * cos(sudut),
            posisi_y + jari_bola * sin(sudut)
        );
    }
    glEnd();
}

void gambarParabola() {
    glColor3f(0, 1, 1);
    glBegin(GL_LINE_STRIP);
    for (double px = -2.5; px <= 2.5; px += 0.01)
        glVertex2f(px, hitungY(px));
    glEnd();
}

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW gagal init!\n";
        return -1;
    }

    GLFWwindow* jendela = glfwCreateWindow(800, 600, "Simulasi Bola U-Shape", NULL, NULL);
    if (!jendela) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(jendela);

    using clock_t = std::chrono::high_resolution_clock;
    auto waktu_sebelumnya = clock_t::now();

    while (!glfwWindowShouldClose(jendela)) {

        auto waktu_sekarang = clock_t::now();
        delta_waktu = std::chrono::duration<double>(waktu_sekarang - waktu_sebelumnya).count();
        waktu_sebelumnya = waktu_sekarang;

        double percepatan_x = hitungPercepatan(posisi_x, kecepatan);
        kecepatan += percepatan_x * delta_waktu;
        kecepatan *= redaman;
        posisi_x += kecepatan * delta_waktu;

        double posisi_y = hitungY(posisi_x);

        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        glScalef(0.5, 0.5, 1);

        gambarParabola();
        gambarBola(posisi_x, posisi_y);

        glfwSwapBuffers(jendela);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
