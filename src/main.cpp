#include <iostream>
#include <chrono>
#include <random>

#include "StormTrack.hpp"

std::vector<double> generate_signal(size_t size, int mask, double amplitude = 20.0, double freq = 0.01) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> noise(0, mask);

    std::vector<double> data(size);
    for (size_t i = 0; i < size; ++i) {
        data[i] = noise(rng) * std::sin(static_cast<double>(i) * freq) * amplitude;
    }
    return data;
}

void ExampleStreaming() {
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    StormTrack window(hInstance, L"[StormTrack] Demo Streaming Data");
    window.Show();

    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> noise(-50, 49);

    std::vector<double> buffer1;
    std::vector<double> buffer2;
    std::vector<double> buffer3;
    std::vector<double> buffer4(10000);

    size_t trace_id1 = window.AddStreamingTrace(L"AM: envelope", RGB(255, 120, 120));
    size_t trace_id2 = window.AddStreamingTrace(L"Damped osc", RGB(120, 255, 120), 0.99);
    size_t trace_id3 = window.AddStreamingTrace(L"FM: envelope", RGB(255, 180, 100), 1.24);
    size_t trace_id4 = window.AddStreamingTrace(L"Noise 10k", RGB(120, 120, 255));

    for (auto& v : buffer4) v = noise(rng);
    window.Streaming(buffer4, trace_id4);

    double t = 0.0;
    double osc_v = 0.0;
    double osc_dv = 100.0;
    int frame_counter = 0;

    auto start = std::chrono::steady_clock::now();
    while (window.IsValid()) {
        t += 0.05;

        double envelope = 50.0 + 40.0 * std::sin(t * 0.1);
        double carrier = std::sin(t * 2.0);
        buffer1.push_back(envelope * carrier);

        if (std::fmod(t, 15.0) < 0.05) {
            osc_dv = 120.0;
        }
        double accel = -0.8 * osc_dv - 3.0 * osc_v;
        osc_dv += accel * 0.05;
        osc_v += osc_dv * 0.05;
        buffer2.push_back(osc_v);

        double mod_freq = 3.0 + 1.0 * sin(t * 0.05);
        double envelope3 = 30.0 * sin(t * mod_freq);
        buffer3.push_back(envelope3);

        if (++frame_counter % 2 == 0) {
            for (size_t i = 0; i < buffer4.size(); ++i) {
                buffer4[i] = noise(rng) + 15.0 * std::sin(t * 0.02 + i * 0.002);
            }
            window.Streaming(buffer4, trace_id4);
        }

        window.Streaming(buffer1, trace_id1);
        window.Streaming(buffer2, trace_id2);
        window.Streaming(buffer3, trace_id3);

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    window.Close();
    window.WaitForClose();
}

void ExampleStatic() {
    std::vector<double> data1 = generate_signal(100000, 0x1f);
    std::vector<double> data2 = generate_signal(100000, 0x1f, 24, 0.17);
    std::vector<double> data3 = generate_signal(100000, 0x1f, 17, 0.02);
    std::vector<double> data4 = generate_signal(1'100, 0x7f);
    std::vector<double> data5 = generate_signal(1'400, 0x7f);

    HINSTANCE hInstance = GetModuleHandle(nullptr);

    StormTrack window(hInstance, L"[StormTrack] Demo Static Data");
    window.StaticData(data1, L"Test 1", RGB(70, 60, 130));
    window.StaticData(data2, L"Test 2", RGB(70, 90, 100));
    window.StaticData(data3, L"Test 3", RGB(70, 130, 90));
    window.Show();

    std::string command;
    while (std::cin >> command) {
        if (command == "exit") {
            break;
        }
    }

    window.Close();
    window.WaitForClose();
}

int main() {
    std::thread streaming(ExampleStreaming);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ExampleStatic();
    streaming.join();
    return 0;
}