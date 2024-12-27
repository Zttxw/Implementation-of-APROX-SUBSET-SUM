

#include "MyForm.h"
using namespace System;
using namespace System::Windows::Forms;


using namespace LAB8;

[STAThreadAttribute]
int main()
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew MyForm());
    return 0;
}
