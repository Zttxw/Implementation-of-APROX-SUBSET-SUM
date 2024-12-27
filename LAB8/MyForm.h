#pragma once

#include <msclr/marshal_cppstd.h> // Para convertir de System::String^ a std::string
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "SubsetSum.h" // Incluir el archivo con el algoritmo

namespace LAB8 {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class MyForm : public System::Windows::Forms::Form
    {
    public:
        MyForm(void)
        {
            InitializeComponent();
        }

    protected:
        ~MyForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::Windows::Forms::TextBox^ txtData;
        System::Windows::Forms::TextBox^ txtTarget;
        System::Windows::Forms::TextBox^ txtEpsilon;
        System::Windows::Forms::Button^ btnCalculate;
        System::Windows::Forms::Label^ lblData;
        System::Windows::Forms::Label^ lblTarget;
        System::Windows::Forms::Label^ lblEpsilon;
        System::Windows::Forms::ListBox^ lstResults;

    private:
        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->txtData = gcnew System::Windows::Forms::TextBox();
            this->txtTarget = gcnew System::Windows::Forms::TextBox();
            this->txtEpsilon = gcnew System::Windows::Forms::TextBox();
            this->btnCalculate = gcnew System::Windows::Forms::Button();
            this->lblData = gcnew System::Windows::Forms::Label();
            this->lblTarget = gcnew System::Windows::Forms::Label();
            this->lblEpsilon = gcnew System::Windows::Forms::Label();
            this->lstResults = gcnew System::Windows::Forms::ListBox();
            this->SuspendLayout();

            // 
            // txtData
            // 
            this->txtData->Location = System::Drawing::Point(150, 30);
            this->txtData->Size = System::Drawing::Size(200, 22);
            this->txtData->TabIndex = 0;

            // 
            // txtTarget
            // 
            this->txtTarget->Location = System::Drawing::Point(150, 70);
            this->txtTarget->Size = System::Drawing::Size(200, 22);
            this->txtTarget->TabIndex = 1;

            // 
            // txtEpsilon
            // 
            this->txtEpsilon->Location = System::Drawing::Point(150, 110);
            this->txtEpsilon->Size = System::Drawing::Size(200, 22);
            this->txtEpsilon->TabIndex = 2;

            // 
            // btnCalculate
            // 
            this->btnCalculate->Location = System::Drawing::Point(150, 150);
            this->btnCalculate->Size = System::Drawing::Size(100, 30);
            this->btnCalculate->Text = L"Calcular";
            this->btnCalculate->TabIndex = 3;
            this->btnCalculate->UseVisualStyleBackColor = true;
            this->btnCalculate->Click += gcnew System::EventHandler(this, &MyForm::btnCalculate_Click);

            // 
            // lblData
            // 
            this->lblData->Location = System::Drawing::Point(30, 30);
            this->lblData->Size = System::Drawing::Size(100, 22);
            this->lblData->Text = L"Conjunto S:";

            // 
            // lblTarget
            // 
            this->lblTarget->Location = System::Drawing::Point(30, 70);
            this->lblTarget->Size = System::Drawing::Size(100, 22);
            this->lblTarget->Text = L"Valor T:";

            // 
            // lblEpsilon
            // 
            this->lblEpsilon->Location = System::Drawing::Point(30, 110);
            this->lblEpsilon->Size = System::Drawing::Size(100, 22);
            this->lblEpsilon->Text = L"Epsilon:";

            // 
            // lstResults
            // 
            this->lstResults->Location = System::Drawing::Point(30, 200);
            this->lstResults->Size = System::Drawing::Size(320, 100);
            this->lstResults->TabIndex = 4;

            // 
            // MyForm
            // 
            this->ClientSize = System::Drawing::Size(400, 350);
            this->Controls->Add(this->txtData);
            this->Controls->Add(this->txtTarget);
            this->Controls->Add(this->txtEpsilon);
            this->Controls->Add(this->btnCalculate);
            this->Controls->Add(this->lblData);
            this->Controls->Add(this->lblTarget);
            this->Controls->Add(this->lblEpsilon);
            this->Controls->Add(this->lstResults);
            this->Text = L"Subset Sum Calculator";
            this->ResumeLayout(false);
            this->PerformLayout();
        }
#pragma endregion

    private: void btnCalculate_Click(System::Object^ sender, System::EventArgs^ e)
    {
        try
        {
            std::vector<double> data;
            double target, epsilon;

            // Leer los valores del conjunto S y asegurarnos de que las comas se reemplacen por puntos
            std::string dataString = msclr::interop::marshal_as<std::string>(this->txtData->Text);
            std::replace(dataString.begin(), dataString.end(), ',', '.');  // Reemplazar comas por puntos

            // Usar stringstream para leer los números desde el texto ingresado
            std::stringstream ss(dataString);
            double num;
            while (ss >> num) {
                data.push_back(num);
            }

            // Verificar si el conjunto S es válido
            if (data.empty()) {
                MessageBox::Show("El conjunto S está vacío o mal formateado. Por favor ingrese valores válidos.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }

            // Leer el objetivo T y la precisión epsilon
            target = std::stod(msclr::interop::marshal_as<std::string>(this->txtTarget->Text));
            epsilon = std::stod(msclr::interop::marshal_as<std::string>(this->txtEpsilon->Text));

            // Mostrar los valores de entrada para verificar que se han recibido correctamente
            std::cout << "Conjunto S: ";
            for (double d : data) std::cout << d << " ";
            std::cout << "\nObjetivo T: " << target << "\nEpsilon: " << epsilon << std::endl;

            // Calcular el resultado usando el algoritmo
            Element result = approximate_subset_sum(data, target, epsilon);

            // Verificar si el resultado tiene un valor cercano al objetivo
            if (result.partials.empty()) {
                MessageBox::Show("No se encontró un subconjunto cercano al objetivo.", "Resultado", MessageBoxButtons::OK, MessageBoxIcon::Information);
                return;
            }

            // Mostrar los resultados en el ListBox
            this->lstResults->Items->Clear();
            this->lstResults->Items->Add("Resultado:");

            // Convertir y mostrar el valor resultante correctamente
            this->lstResults->Items->Add("Valor: " + result.value.ToString("F2"));  // Mostrar con 2 decimales

            // Mostrar el subconjunto de forma adecuada
            this->lstResults->Items->Add("Subconjunto:");
            for (double p : result.partials) {
                this->lstResults->Items->Add(p.ToString("F2"));  // Mostrar cada elemento con 2 decimales
            }

        }
        catch (...)
        {
            MessageBox::Show("Error al procesar los datos. Verifique su entrada.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }

    };
}
