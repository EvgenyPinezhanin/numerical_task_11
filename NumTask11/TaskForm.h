#pragma once

namespace NumTask11 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// ������ ��� TaskForm
	/// </summary>
	public ref class TaskForm : public System::Windows::Forms::Form
	{
	public:
		Resources::ResourceManager^ s_pxResourceManager;

		TaskForm(void)
		{
			InitializeComponent();
			//
			//TODO: �������� ��� ������������
			//

			Reflection::Assembly^ pxAssembly = Reflection::Assembly::GetExecutingAssembly();
			String^ pxResName = pxAssembly->GetName()->Name + ".resource";
			s_pxResourceManager = (gcnew Resources::ResourceManager(pxResName, pxAssembly));
			PictureTask->Image = (cli::safe_cast<Drawing::Bitmap^>(s_pxResourceManager->GetObject("Task11")));
		}

	protected:
		/// <summary>
		/// ���������� ��� ������������ �������.
		/// </summary>
		~TaskForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^ PictureTask;
	private: System::Windows::Forms::Button^ OKButton;

	private:
		/// <summary>
		/// ������������ ���������� ������������.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// ��������� ����� ��� ��������� ������������ � �� ��������� 
		/// ���������� ����� ������ � ������� ��������� ����.
		/// </summary>
		void InitializeComponent(void)
		{
			this->PictureTask = (gcnew System::Windows::Forms::PictureBox());
			this->OKButton = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PictureTask))->BeginInit();
			this->SuspendLayout();
			// 
			// PictureTask
			// 
			this->PictureTask->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->PictureTask->Location = System::Drawing::Point(12, 12);
			this->PictureTask->Name = L"PictureTask";
			this->PictureTask->Size = System::Drawing::Size(827, 578);
			this->PictureTask->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->PictureTask->TabIndex = 0;
			this->PictureTask->TabStop = false;
			// 
			// OKButton
			// 
			this->OKButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->OKButton->Location = System::Drawing::Point(727, 596);
			this->OKButton->Name = L"OKButton";
			this->OKButton->Size = System::Drawing::Size(112, 58);
			this->OKButton->TabIndex = 1;
			this->OKButton->Text = L"OK";
			this->OKButton->UseVisualStyleBackColor = true;
			this->OKButton->Click += gcnew System::EventHandler(this, &TaskForm::OKButton_Click);
			// 
			// TaskForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(851, 666);
			this->Controls->Add(this->OKButton);
			this->Controls->Add(this->PictureTask);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"TaskForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"������� ������";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PictureTask))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void OKButton_Click(System::Object^ sender, System::EventArgs^ e) {
		TaskForm::Close();
	}
	};
}
