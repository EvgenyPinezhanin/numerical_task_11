#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

[System::Runtime::InteropServices::DllImport("user32.dll")]
extern bool SetProcessDPIAware();

[STAThreadAttribute]

void main(cli::array<System::String^>^ args) {
    SetProcessDPIAware();
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    NumTask11::MainForm form;
    Application::Run(% form);
}