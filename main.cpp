#include "imgui.h"
#include "implot.h"
#include "imgui_stdlib.h"
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <algorithm>

#include <iostream>

#include "crypto.h"
#include "window.h"
#include "util.h"

using namespace std;

struct caesar_data {
	string clear = "";
	string ciffer = "";
	int index = 1;
} c_data;

struct vigenere_data {
	string clear = "";
	string ciffer = "";
	string key = "";
	vector<uint> kasiski;
	int kasiski_min = 2;
	int kasiski_max = 4;
} v_data;
const char* abc[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
const double zero_to_25[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};


struct Callbacks {
	static int V_CifferCallback(ImGuiInputTextCallbackData* data) {
		if(data->EventFlag == ImGuiInputTextFlags_CallbackEdit)
		{
			v_data.clear = Crypto::Vigenere::decrypt(data->Buf, v_data.key);
		}
		return 0;
	}
	static int V_ClearCallback(ImGuiInputTextCallbackData* data) {
		if(data->EventFlag == ImGuiInputTextFlags_CallbackEdit)
		{
			v_data.ciffer = Crypto::Vigenere::encrypt(data->Buf, v_data.key);
		}
		return 0;
	}
	static int C_CifferCallback(ImGuiInputTextCallbackData* data) {
		if(data->EventFlag == ImGuiInputTextFlags_CallbackEdit)
		{
			c_data.clear = Crypto::Caesar::decrypt(data->Buf, c_data.index);
		}
		return 0;
	}
	static int C_ClearCallback(ImGuiInputTextCallbackData* data) {
		if(data->EventFlag == ImGuiInputTextFlags_CallbackEdit)
		{
			c_data.ciffer = Crypto::Caesar::encrypt(data->Buf, c_data.index);
		}
		return 0;
	}
};

int main(int, char**)
{
	if(Window::init() != 0)
		return 1;

	Crypto::calculate_prime_factors();

    // Main loop
    while (!glfwWindowShouldClose(Window::window))
    {
		Window::update();

        //ImGui::ShowDemoWindow();
        //ImPlot::ShowDemoWindow();
		// Cäsar
		{
			ImGui::Begin("Cäsar");
			ImGui::InputTextMultiline("Klartext", &c_data.clear, ImVec2(0,0), ImGuiInputTextFlags_CallbackEdit, Callbacks::C_ClearCallback);
			ImGui::InputTextMultiline("Ciffer-Text", &c_data.ciffer, ImVec2(0,0), ImGuiInputTextFlags_CallbackEdit, Callbacks::C_CifferCallback);
			ImGui::SliderInt("Cäsar verschiebung", &c_data.index, 1, 25);
			if(ImGui::Button("Verschlüsseln"))
				c_data.ciffer = Crypto::Caesar::encrypt(c_data.clear, c_data.index);
			ImGui::SameLine();
			if(ImGui::Button("Entschlüsseln"))
				c_data.clear = Crypto::Caesar::decrypt(c_data.ciffer, c_data.index);

			if(c_data.ciffer.length() > 0) {
				uint* data = Crypto::Caesar::analyze(c_data.ciffer);
				ImPlot::BeginPlot("Häufigkeitsanalyse");
            	ImPlot::SetupAxes("Anzahl","Buchstabe",ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);
            	ImPlot::SetupAxisTicks(ImAxis_X1,zero_to_25, 26, abc);
				ImPlot::PlotBars("Label", data, 26, .7, 1);
				ImPlot::EndPlot();
				free(data);
			}
			ImGui::End();
		}

		{
			ImGui::Begin("Vigenére");
			ImGui::InputTextMultiline("Klartext", &v_data.clear, ImVec2(0,0), ImGuiInputTextFlags_CallbackEdit, Callbacks::V_ClearCallback);
			ImGui::InputTextMultiline("Ciffer-Text", &v_data.ciffer, ImVec2(0,0), ImGuiInputTextFlags_CallbackEdit, Callbacks::V_CifferCallback);
			ImGui::InputText("Schlüssel", &v_data.key);
			if(ImGui::Button("Verschlüsseln"))
				v_data.ciffer = Crypto::Vigenere::encrypt(v_data.clear, v_data.key);
			ImGui::SameLine();
			if(ImGui::Button("Entschlüsseln"))
				v_data.clear = Crypto::Vigenere::decrypt(v_data.ciffer, v_data.key);

			if(v_data.ciffer.length() > 0 && v_data.key.length() > 0) {
				vector<vector<uint>> data = Crypto::Vigenere::analyze(v_data.ciffer, v_data.key.length());

				vector<string> labels;
				for(uint i=0; i<data.size(); i++)
					labels.push_back(to_string(i) + string("-tes Element"));

				ImPlot::BeginPlot("Häufigkeitsanalyse");
				ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside);
            	ImPlot::SetupAxes("Anzahl","Buchstabe",ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);
            	ImPlot::SetupAxisTicks(ImAxis_X1,zero_to_25, 26, abc);
				ImPlotUtil::PlotBarGroups(labels, data);
				ImPlot::EndPlot();
			}
			if(v_data.ciffer.length() > 0)
				ImGui::Text("Friedmans-Test: h=%f", Crypto::Vigenere::friedman_de(v_data.ciffer));
			//TODO: Make the maximal and minimal pattern length being set manually
			if(ImGui::Button("Kasiski-Test starten"))
				v_data.kasiski = Crypto::Vigenere::kasiski_count(v_data.ciffer, 3);
			ImGui::SameLine();
			ImGui::PushItemWidth(100);
			ImGui::SliderInt("Min Länge", &v_data.kasiski_min, 2, 100);
			ImGui::SameLine();
			ImGui::SliderInt("Max Länge", &v_data.kasiski_max, v_data.kasiski_min, 100);
			if(v_data.kasiski_min > v_data.kasiski_max) v_data.kasiski_max = v_data.kasiski_min;
			ImGui::PopItemWidth();
			ImPlot::BeginPlot("Kasiski-Test");
			ImPlot::SetupAxes("Schlüssellänge","Vorkommen",ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_LogScale, ImPlotAxisFlags_AutoFit);
			ImPlot::PlotBars("Kasiski-Vorkommnisse", v_data.kasiski.data(), v_data.kasiski.size());
			ImPlot::EndPlot();
			uint index = distance(v_data.kasiski.begin(), max_element(v_data.kasiski.begin(), v_data.kasiski.end()));
			ImGui::Text("Die höchste Warscheinlichkeit liegt bei %u", index);
			ImGui::End();
		}
		Window::render();
    }
	Window::stop();
    return 0;
}
