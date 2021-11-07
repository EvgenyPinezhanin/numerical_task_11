#pragma once

namespace NumTask11 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// —водка дл€ MethodForm
	/// </summary>
	public ref class MethodForm : public System::Windows::Forms::Form
	{
	public:

		Resources::ResourceManager^ s_pxResourceManager;

		MethodForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//

			Reflection::Assembly^ pxAssembly = Reflection::Assembly::GetExecutingAssembly();
			String^ pxResName = pxAssembly->GetName()->Name + ".resource";
			s_pxResourceManager = (gcnew Resources::ResourceManager(pxResName, pxAssembly));
			PictureMethod->Image = (cli::safe_cast<Drawing::Bitmap^>(s_pxResourceManager->GetObject("methodRKM")));
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~MethodForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^ PictureMethod;
	private: System::Windows::Forms::Button^ OKButton;

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->PictureMethod = (gcnew System::Windows::Forms::PictureBox());
			this->OKButton = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PictureMethod))->BeginInit();
			this->SuspendLayout();
			// 
			// PictureMethod
			// 
			this->PictureMethod->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->PictureMethod->Location = System::Drawing::Point(12, 12);
			this->PictureMethod->Name = L"PictureMethod";
			this->PictureMethod->Size = System::Drawing::Size(803, 522);
			this->PictureMethod->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->PictureMethod->TabIndex = 0;
			this->PictureMethod->TabStop = false;
			// 
			// OKButton
			// 
			this->OKButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->OKButton->Location = System::Drawing::Point(703, 540);
			this->OKButton->Name = L"OKButton";
			this->OKButton->Size = System::Drawing::Size(112, 58);
			this->OKButton->TabIndex = 1;
			this->OKButton->Text = L"OK";
			this->OKButton->UseVisualStyleBackColor = true;
			this->OKButton->Click += gcnew System::EventHandler(this, &MethodForm::OKButton_Click);
			// 
			// MethodForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(827, 610);
			this->Controls->Add(this->OKButton);
			this->Controls->Add(this->PictureMethod);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"MethodForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"ќ методе";
			this->Closing  += gcnew CancelEventHandler(this, &MethodForm::MethodForm_Closing);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PictureMethod))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void OKButton_Click(System::Object^ sender, System::EventArgs^ e) {
		MethodForm::Hide();
	}

	private: System::Void MethodForm_Closing(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e) {
		e->Cancel = true;
		MethodForm::Hide();
	}
	};
}
