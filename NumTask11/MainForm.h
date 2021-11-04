#pragma once
#include"TaskForm.h"
#include"MethodForm.h"

namespace NumTask11 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^ MenuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^ StartMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ TaskMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ MethodMenuItem;



	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->MenuStrip = (gcnew System::Windows::Forms::MenuStrip());
			this->StartMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TaskMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MethodMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MenuStrip->SuspendLayout();
			this->SuspendLayout();
			// 
			// MenuStrip
			// 
			this->MenuStrip->ImageScalingSize = System::Drawing::Size(20, 20);
			this->MenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->StartMenuItem, this->TaskMenuItem,
					this->MethodMenuItem
			});
			this->MenuStrip->Location = System::Drawing::Point(0, 0);
			this->MenuStrip->Name = L"MenuStrip";
			this->MenuStrip->Size = System::Drawing::Size(1661, 28);
			this->MenuStrip->TabIndex = 0;
			this->MenuStrip->Text = L"menuStrip1";
			// 
			// StartMenuItem
			// 
			this->StartMenuItem->Name = L"StartMenuItem";
			this->StartMenuItem->Size = System::Drawing::Size(69, 24);
			this->StartMenuItem->Text = L"Запуск";
			this->StartMenuItem->Click += gcnew System::EventHandler(this, &MainForm::StartMenuItem_Click);
			// 
			// TaskMenuItem
			// 
			this->TaskMenuItem->Name = L"TaskMenuItem";
			this->TaskMenuItem->Size = System::Drawing::Size(133, 24);
			this->TaskMenuItem->Text = L"Условия задачи";
			this->TaskMenuItem->Click += gcnew System::EventHandler(this, &MainForm::TaskMenuItem_Click);
			// 
			// MethodMenuItem
			// 
			this->MethodMenuItem->Name = L"MethodMenuItem";
			this->MethodMenuItem->Size = System::Drawing::Size(88, 24);
			this->MethodMenuItem->Text = L"О методе";
			this->MethodMenuItem->Click += gcnew System::EventHandler(this, &MainForm::MethodMenuItem_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1661, 936);
			this->Controls->Add(this->MenuStrip);
			this->MainMenuStrip = this->MenuStrip;
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"MainForm";
			this->MenuStrip->ResumeLayout(false);
			this->MenuStrip->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void StartMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {

	}
	private: System::Void TaskMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		TaskForm^ tForm = gcnew TaskForm();
		tForm->Show();
	}
	private: System::Void MethodMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		MethodForm^ mForm = gcnew MethodForm();
		mForm->Show();
	}
};
}
