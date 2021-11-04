#pragma once
#include<cmath>
#include<vector>
#include"TaskForm.h"
#include"MethodForm.h"
#include"numerical.h"

using namespace std;

namespace NumTask11 {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Reflection;

	// Функция для установки Control двойной буфферизации
	void SetDoubleBuffered(Control^ c, bool value) {
		PropertyInfo^ pi = (Control::typeid)->GetProperty("DoubleBuffered", BindingFlags::SetProperty | BindingFlags::Instance | BindingFlags::NonPublic);
		if (pi != nullptr) {
			pi->SetValue(c, value, nullptr);
		}
	}

	// Функция для проверки, что переменная типа double лежит в диапазоне чисел decimal
	// Нужна при рисовании точек на графике
	bool double_in_dec(double d) {
		static double maxD = 79228162514264337593543950335.0;
		static double minD = 0.00000000000000000000000000001;
		if ((abs(d) >= minD && abs(d) <= maxD) || d == 0.0) {
			return true;
		}
		return false;
	}

	// Функция signum
	template <typename T> int sgn(T val) {
		return (T(0) < val) - (val < T(0));
	}

	double m, k, f;
	double x0, u0, ud0;
	double g = 9.81;
	double C1, C2;

	//переделать
	void calc_C1() {
		C1 = 0.0;
	}

	//переделать
	void calc_C2() {
		C2 = m * g / k + u0;
	}

	double ud(double x, double u1, double u2) {
		return u2;
	}

	double udd(double x, double u1, double u2) {
		return -sgn(u2) * f * g - k / m * u1;
	}

	double f2(double x) {
		return sqrt(k / m) * C1 * cos(sqrt(k / m) * x) - sqrt(k / m) * C2 * sin(sqrt(k / m) * x);
	}

	double f1(double x) {
		return C1 * sin(sqrt(k / m) * x) + C2 * cos(sqrt(k / m) * x) - sgn(f2(x)) * m * g / k;
	}

	/// <summary>
	/// Сводка для MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		double h0;
		int Nmax;
		double b, Egr, E, Emin;

		vector<double>* X, * OLP_Arr, * H, * U_V;
		vector<vector<double>>* V, * V_cap, * V_res, * U;
		vector<int>* step_dec, * step_inc;
		rkm_method* rkm;

		CONTROL control;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ Chart1;
	public:

	private: System::Windows::Forms::DataVisualization::Charting::Chart^ Chart2;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ Chart3;
	private: System::Windows::Forms::ToolTip^ mToolTip;
	private: System::Windows::Forms::ToolTip^ kToolTip;
	private: System::Windows::Forms::ToolTip^ fToolTip;
	private: System::Windows::Forms::ToolTip^ h0ToolTip;
	private: System::Windows::Forms::ToolTip^ NmaxToolTip;
	private: System::Windows::Forms::ToolTip^ bToolTip;
	private: System::Windows::Forms::ToolTip^ EgrToolTip;
	private: System::Windows::Forms::ToolTip^ EminToolTip;




	public:

		   VNEXT vnext;

		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//

			rkm = new rkm_method(ud, udd);
			ControlComboBox->SelectedIndex = 0;
			VnComboBox->SelectedIndex = 0;
			control = UPDOWN;
			vnext = VN;

			X = new vector<double>;
			OLP_Arr = new vector<double>;
			H = new vector<double>;
			U_V = new vector<double>;
			V = new vector<vector<double>>;
			V_cap = new vector<vector<double>>;
			V_res = new vector<vector<double>>;
			U = new vector<vector<double>>;
			step_dec = new vector<int>;
			step_inc = new vector<int>;

			SetDoubleBuffered(Table, true);
			
			//добавить
			mToolTip->SetToolTip(mLabel, "Масса груза");
			kToolTip->SetToolTip(kLabel, "Жесткость пружины");
			fToolTip->SetToolTip(fLabel, "Коэффициент пропорциональности силы трения");
			h0ToolTip->SetToolTip(h0Label, "Начальный шаг");
			NmaxToolTip->SetToolTip(NMaxLabel, "Максимально допустимое число шагов");
			bToolTip->SetToolTip(BLabel, "Правая граница учатка интегрирования");
			EgrToolTip->SetToolTip(EgrLabel, "Параметр контроля выхода на правую границу");
			EminToolTip->SetToolTip(EminLabel, "Параметр контроля локальной погрешности \"снизу\"");
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
	private: System::Windows::Forms::GroupBox^ ParamSystBox;
	private: System::Windows::Forms::TextBox^ fTextBox;
	private: System::Windows::Forms::TextBox^ kTextBox;
	private: System::Windows::Forms::TextBox^ mTextBox;
	private: System::Windows::Forms::Label^ fLabel;
	private: System::Windows::Forms::Label^ kLabel;
	private: System::Windows::Forms::Label^ mLabel;
	private: System::Windows::Forms::GroupBox^ NUBox;
	private: System::Windows::Forms::TextBox^ Ud0TextBox;
	private: System::Windows::Forms::TextBox^ U0TextBox;
	private: System::Windows::Forms::TextBox^ X0TextBox;
	private: System::Windows::Forms::Label^ Ud0Label;
	private: System::Windows::Forms::Label^ U0Label;
	private: System::Windows::Forms::Label^ X0label;
	private: System::Windows::Forms::GroupBox^ ParamMethodBox;
	private: System::Windows::Forms::TextBox^ h0TextBox;
	private: System::Windows::Forms::Label^ h0Label;
	private: System::Windows::Forms::GroupBox^ ControlBox;
	private: System::Windows::Forms::ComboBox^ VnComboBox;
	private: System::Windows::Forms::ComboBox^ ControlComboBox;
	private: System::Windows::Forms::TextBox^ EminTextBox;
	private: System::Windows::Forms::TextBox^ ETextBox;
	private: System::Windows::Forms::TextBox^ EgrTextBox;
	private: System::Windows::Forms::TextBox^ BTextBox;
	private: System::Windows::Forms::TextBox^ NmaxTextBox;
	private: System::Windows::Forms::Label^ EminLabel;
	private: System::Windows::Forms::Label^ ELabel;
	private: System::Windows::Forms::Label^ EgrLabel;
	private: System::Windows::Forms::Label^ BLabel;
	private: System::Windows::Forms::Label^ NMaxLabel;
private: System::ComponentModel::IContainer^ components;



	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^ series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Title^ title1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea2 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^ series4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Title^ title2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea3 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series5 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^ series6 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Title^ title3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
			this->MenuStrip = (gcnew System::Windows::Forms::MenuStrip());
			this->StartMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TaskMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MethodMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ParamSystBox = (gcnew System::Windows::Forms::GroupBox());
			this->fTextBox = (gcnew System::Windows::Forms::TextBox());
			this->kTextBox = (gcnew System::Windows::Forms::TextBox());
			this->mTextBox = (gcnew System::Windows::Forms::TextBox());
			this->fLabel = (gcnew System::Windows::Forms::Label());
			this->kLabel = (gcnew System::Windows::Forms::Label());
			this->mLabel = (gcnew System::Windows::Forms::Label());
			this->NUBox = (gcnew System::Windows::Forms::GroupBox());
			this->Ud0TextBox = (gcnew System::Windows::Forms::TextBox());
			this->U0TextBox = (gcnew System::Windows::Forms::TextBox());
			this->X0TextBox = (gcnew System::Windows::Forms::TextBox());
			this->Ud0Label = (gcnew System::Windows::Forms::Label());
			this->U0Label = (gcnew System::Windows::Forms::Label());
			this->X0label = (gcnew System::Windows::Forms::Label());
			this->ParamMethodBox = (gcnew System::Windows::Forms::GroupBox());
			this->h0TextBox = (gcnew System::Windows::Forms::TextBox());
			this->h0Label = (gcnew System::Windows::Forms::Label());
			this->ControlBox = (gcnew System::Windows::Forms::GroupBox());
			this->VnComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->ControlComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->EminTextBox = (gcnew System::Windows::Forms::TextBox());
			this->ETextBox = (gcnew System::Windows::Forms::TextBox());
			this->EgrTextBox = (gcnew System::Windows::Forms::TextBox());
			this->BTextBox = (gcnew System::Windows::Forms::TextBox());
			this->NmaxTextBox = (gcnew System::Windows::Forms::TextBox());
			this->EminLabel = (gcnew System::Windows::Forms::Label());
			this->ELabel = (gcnew System::Windows::Forms::Label());
			this->EgrLabel = (gcnew System::Windows::Forms::Label());
			this->BLabel = (gcnew System::Windows::Forms::Label());
			this->NMaxLabel = (gcnew System::Windows::Forms::Label());
			this->Chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->Chart2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->Chart3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->mToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->kToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->fToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->h0ToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->NmaxToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->bToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->EgrToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->EminToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->MenuStrip->SuspendLayout();
			this->ParamSystBox->SuspendLayout();
			this->NUBox->SuspendLayout();
			this->ParamMethodBox->SuspendLayout();
			this->ControlBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Chart1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Chart2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Chart3))->BeginInit();
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
			// ParamSystBox
			// 
			this->ParamSystBox->Controls->Add(this->fTextBox);
			this->ParamSystBox->Controls->Add(this->kTextBox);
			this->ParamSystBox->Controls->Add(this->mTextBox);
			this->ParamSystBox->Controls->Add(this->fLabel);
			this->ParamSystBox->Controls->Add(this->kLabel);
			this->ParamSystBox->Controls->Add(this->mLabel);
			this->ParamSystBox->Location = System::Drawing::Point(12, 31);
			this->ParamSystBox->Name = L"ParamSystBox";
			this->ParamSystBox->Size = System::Drawing::Size(179, 117);
			this->ParamSystBox->TabIndex = 4;
			this->ParamSystBox->TabStop = false;
			this->ParamSystBox->Text = L"Параметры системы";
			// 
			// fTextBox
			// 
			this->fTextBox->Location = System::Drawing::Point(31, 77);
			this->fTextBox->Name = L"fTextBox";
			this->fTextBox->Size = System::Drawing::Size(123, 22);
			this->fTextBox->TabIndex = 5;
			this->fTextBox->Text = L"0.3";
			// 
			// kTextBox
			// 
			this->kTextBox->Location = System::Drawing::Point(31, 49);
			this->kTextBox->Name = L"kTextBox";
			this->kTextBox->Size = System::Drawing::Size(123, 22);
			this->kTextBox->TabIndex = 4;
			this->kTextBox->Text = L"175";
			// 
			// mTextBox
			// 
			this->mTextBox->Location = System::Drawing::Point(31, 21);
			this->mTextBox->Name = L"mTextBox";
			this->mTextBox->Size = System::Drawing::Size(123, 22);
			this->mTextBox->TabIndex = 3;
			this->mTextBox->Text = L"4.5";
			// 
			// fLabel
			// 
			this->fLabel->AutoSize = true;
			this->fLabel->Location = System::Drawing::Point(8, 80);
			this->fLabel->Name = L"fLabel";
			this->fLabel->Size = System::Drawing::Size(12, 17);
			this->fLabel->TabIndex = 2;
			this->fLabel->Text = L"f";
			// 
			// kLabel
			// 
			this->kLabel->AutoSize = true;
			this->kLabel->Location = System::Drawing::Point(8, 52);
			this->kLabel->Name = L"kLabel";
			this->kLabel->Size = System::Drawing::Size(15, 17);
			this->kLabel->TabIndex = 1;
			this->kLabel->Text = L"k";
			// 
			// mLabel
			// 
			this->mLabel->AutoSize = true;
			this->mLabel->Location = System::Drawing::Point(6, 24);
			this->mLabel->Name = L"mLabel";
			this->mLabel->Size = System::Drawing::Size(19, 17);
			this->mLabel->TabIndex = 0;
			this->mLabel->Text = L"m";
			// 
			// NUBox
			// 
			this->NUBox->Controls->Add(this->Ud0TextBox);
			this->NUBox->Controls->Add(this->U0TextBox);
			this->NUBox->Controls->Add(this->X0TextBox);
			this->NUBox->Controls->Add(this->Ud0Label);
			this->NUBox->Controls->Add(this->U0Label);
			this->NUBox->Controls->Add(this->X0label);
			this->NUBox->Location = System::Drawing::Point(197, 31);
			this->NUBox->Name = L"NUBox";
			this->NUBox->Size = System::Drawing::Size(200, 117);
			this->NUBox->TabIndex = 5;
			this->NUBox->TabStop = false;
			this->NUBox->Text = L"Начальные условия";
			// 
			// Ud0TextBox
			// 
			this->Ud0TextBox->Location = System::Drawing::Point(31, 75);
			this->Ud0TextBox->Name = L"Ud0TextBox";
			this->Ud0TextBox->Size = System::Drawing::Size(146, 22);
			this->Ud0TextBox->TabIndex = 5;
			this->Ud0TextBox->Text = L"0";
			// 
			// U0TextBox
			// 
			this->U0TextBox->Location = System::Drawing::Point(31, 47);
			this->U0TextBox->Name = L"U0TextBox";
			this->U0TextBox->Size = System::Drawing::Size(146, 22);
			this->U0TextBox->TabIndex = 4;
			this->U0TextBox->Text = L"7.5";
			// 
			// X0TextBox
			// 
			this->X0TextBox->Location = System::Drawing::Point(31, 19);
			this->X0TextBox->Name = L"X0TextBox";
			this->X0TextBox->Size = System::Drawing::Size(146, 22);
			this->X0TextBox->TabIndex = 3;
			this->X0TextBox->Text = L"0";
			// 
			// Ud0Label
			// 
			this->Ud0Label->AutoSize = true;
			this->Ud0Label->Location = System::Drawing::Point(6, 75);
			this->Ud0Label->Name = L"Ud0Label";
			this->Ud0Label->Size = System::Drawing::Size(24, 17);
			this->Ud0Label->TabIndex = 2;
			this->Ud0Label->Text = L"u\'₀";
			// 
			// U0Label
			// 
			this->U0Label->AutoSize = true;
			this->U0Label->Location = System::Drawing::Point(6, 50);
			this->U0Label->Name = L"U0Label";
			this->U0Label->Size = System::Drawing::Size(21, 17);
			this->U0Label->TabIndex = 1;
			this->U0Label->Text = L"u₀";
			// 
			// X0label
			// 
			this->X0label->AutoSize = true;
			this->X0label->Location = System::Drawing::Point(6, 22);
			this->X0label->Name = L"X0label";
			this->X0label->Size = System::Drawing::Size(19, 17);
			this->X0label->TabIndex = 0;
			this->X0label->Text = L"x₀";
			// 
			// ParamMethodBox
			// 
			this->ParamMethodBox->Controls->Add(this->h0TextBox);
			this->ParamMethodBox->Controls->Add(this->h0Label);
			this->ParamMethodBox->Location = System::Drawing::Point(12, 154);
			this->ParamMethodBox->Name = L"ParamMethodBox";
			this->ParamMethodBox->Size = System::Drawing::Size(385, 57);
			this->ParamMethodBox->TabIndex = 7;
			this->ParamMethodBox->TabStop = false;
			this->ParamMethodBox->Text = L"Параметры метода";
			// 
			// h0TextBox
			// 
			this->h0TextBox->Location = System::Drawing::Point(48, 21);
			this->h0TextBox->Name = L"h0TextBox";
			this->h0TextBox->Size = System::Drawing::Size(314, 22);
			this->h0TextBox->TabIndex = 1;
			this->h0TextBox->Text = L"0.0001";
			// 
			// h0Label
			// 
			this->h0Label->AutoSize = true;
			this->h0Label->Location = System::Drawing::Point(14, 24);
			this->h0Label->Name = L"h0Label";
			this->h0Label->Size = System::Drawing::Size(21, 17);
			this->h0Label->TabIndex = 0;
			this->h0Label->Text = L"h₀";
			// 
			// ControlBox
			// 
			this->ControlBox->Controls->Add(this->VnComboBox);
			this->ControlBox->Controls->Add(this->ControlComboBox);
			this->ControlBox->Controls->Add(this->EminTextBox);
			this->ControlBox->Controls->Add(this->ETextBox);
			this->ControlBox->Controls->Add(this->EgrTextBox);
			this->ControlBox->Controls->Add(this->BTextBox);
			this->ControlBox->Controls->Add(this->NmaxTextBox);
			this->ControlBox->Controls->Add(this->EminLabel);
			this->ControlBox->Controls->Add(this->ELabel);
			this->ControlBox->Controls->Add(this->EgrLabel);
			this->ControlBox->Controls->Add(this->BLabel);
			this->ControlBox->Controls->Add(this->NMaxLabel);
			this->ControlBox->Location = System::Drawing::Point(12, 217);
			this->ControlBox->Name = L"ControlBox";
			this->ControlBox->Size = System::Drawing::Size(385, 227);
			this->ControlBox->TabIndex = 8;
			this->ControlBox->TabStop = false;
			this->ControlBox->Text = L"Контроль";
			// 
			// VnComboBox
			// 
			this->VnComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->VnComboBox->FormattingEnabled = true;
			this->VnComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) {
				L"В качестве Vₙ итог берем Vₙ", L"В качестве Vₙ итог берем V̂ₙ",
					L"В качестве Vₙ итог берем Vₙ кор"
			});
			this->VnComboBox->Location = System::Drawing::Point(9, 191);
			this->VnComboBox->Name = L"VnComboBox";
			this->VnComboBox->Size = System::Drawing::Size(353, 24);
			this->VnComboBox->TabIndex = 11;
			// 
			// ControlComboBox
			// 
			this->ControlComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->ControlComboBox->FormattingEnabled = true;
			this->ControlComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) {
				L"Контроль погрешности \"сверху\" и \"снизу\"",
					L"Отказ от контроля погрешности \"снизу\"", L"Отказ от контроля \"снизу\" и \"сверху\""
			});
			this->ControlComboBox->Location = System::Drawing::Point(9, 161);
			this->ControlComboBox->Name = L"ControlComboBox";
			this->ControlComboBox->Size = System::Drawing::Size(353, 24);
			this->ControlComboBox->TabIndex = 10;
			// 
			// EminTextBox
			// 
			this->EminTextBox->Location = System::Drawing::Point(48, 133);
			this->EminTextBox->Name = L"EminTextBox";
			this->EminTextBox->Size = System::Drawing::Size(314, 22);
			this->EminTextBox->TabIndex = 9;
			this->EminTextBox->Text = L"0.0000005";
			// 
			// ETextBox
			// 
			this->ETextBox->Location = System::Drawing::Point(48, 105);
			this->ETextBox->Name = L"ETextBox";
			this->ETextBox->Size = System::Drawing::Size(314, 22);
			this->ETextBox->TabIndex = 8;
			this->ETextBox->Text = L"0.000005";
			// 
			// EgrTextBox
			// 
			this->EgrTextBox->Location = System::Drawing::Point(48, 77);
			this->EgrTextBox->Name = L"EgrTextBox";
			this->EgrTextBox->Size = System::Drawing::Size(314, 22);
			this->EgrTextBox->TabIndex = 7;
			this->EgrTextBox->Text = L"0.000005";
			// 
			// BTextBox
			// 
			this->BTextBox->Location = System::Drawing::Point(48, 49);
			this->BTextBox->Name = L"BTextBox";
			this->BTextBox->Size = System::Drawing::Size(314, 22);
			this->BTextBox->TabIndex = 6;
			this->BTextBox->Text = L"2.0";
			// 
			// NmaxTextBox
			// 
			this->NmaxTextBox->Location = System::Drawing::Point(48, 21);
			this->NmaxTextBox->Name = L"NmaxTextBox";
			this->NmaxTextBox->Size = System::Drawing::Size(314, 22);
			this->NmaxTextBox->TabIndex = 5;
			this->NmaxTextBox->Text = L"10000";
			// 
			// EminLabel
			// 
			this->EminLabel->AutoSize = true;
			this->EminLabel->Location = System::Drawing::Point(8, 136);
			this->EminLabel->Name = L"EminLabel";
			this->EminLabel->Size = System::Drawing::Size(32, 17);
			this->EminLabel->TabIndex = 4;
			this->EminLabel->Text = L"Eₘᵢₙ";
			// 
			// ELabel
			// 
			this->ELabel->AutoSize = true;
			this->ELabel->Location = System::Drawing::Point(14, 108);
			this->ELabel->Name = L"ELabel";
			this->ELabel->Size = System::Drawing::Size(17, 17);
			this->ELabel->TabIndex = 3;
			this->ELabel->Text = L"E";
			// 
			// EgrLabel
			// 
			this->EgrLabel->AutoSize = true;
			this->EgrLabel->Location = System::Drawing::Point(8, 80);
			this->EgrLabel->Name = L"EgrLabel";
			this->EgrLabel->Size = System::Drawing::Size(30, 17);
			this->EgrLabel->TabIndex = 2;
			this->EgrLabel->Text = L"Eгр";
			// 
			// BLabel
			// 
			this->BLabel->AutoSize = true;
			this->BLabel->Location = System::Drawing::Point(14, 52);
			this->BLabel->Name = L"BLabel";
			this->BLabel->Size = System::Drawing::Size(16, 17);
			this->BLabel->TabIndex = 1;
			this->BLabel->Text = L"b";
			// 
			// NMaxLabel
			// 
			this->NMaxLabel->AutoSize = true;
			this->NMaxLabel->Location = System::Drawing::Point(6, 24);
			this->NMaxLabel->Name = L"NMaxLabel";
			this->NMaxLabel->Size = System::Drawing::Size(36, 17);
			this->NMaxLabel->TabIndex = 0;
			this->NMaxLabel->Text = L"Nₘₐₓ";
			// 
			// Chart1
			// 
			chartArea1->Name = L"ChartArea1";
			this->Chart1->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->Chart1->Legends->Add(legend1);
			this->Chart1->Location = System::Drawing::Point(60, 520);
			this->Chart1->Name = L"Chart1";
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series1->Legend = L"Legend1";
			series1->Name = L"Истинное решение";
			series2->ChartArea = L"ChartArea1";
			series2->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series2->Legend = L"Legend1";
			series2->Name = L"Численное решение";
			this->Chart1->Series->Add(series1);
			this->Chart1->Series->Add(series2);
			this->Chart1->Size = System::Drawing::Size(300, 300);
			this->Chart1->TabIndex = 9;
			this->Chart1->Text = L"Chart1";
			title1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			title1->Name = L"Title1";
			title1->Text = L"График зависимости v(x)";
			this->Chart1->Titles->Add(title1);
			// 
			// Chart2
			// 
			chartArea2->Name = L"ChartArea1";
			this->Chart2->ChartAreas->Add(chartArea2);
			legend2->Name = L"Legend1";
			this->Chart2->Legends->Add(legend2);
			this->Chart2->Location = System::Drawing::Point(366, 520);
			this->Chart2->Name = L"Chart2";
			series3->ChartArea = L"ChartArea1";
			series3->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series3->Legend = L"Legend1";
			series3->Name = L"Истинное решение";
			series4->ChartArea = L"ChartArea1";
			series4->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series4->Legend = L"Legend1";
			series4->Name = L"Численное решение";
			this->Chart2->Series->Add(series3);
			this->Chart2->Series->Add(series4);
			this->Chart2->Size = System::Drawing::Size(300, 300);
			this->Chart2->TabIndex = 10;
			this->Chart2->Text = L"Chart2";
			title2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			title2->Name = L"Title1";
			title2->Text = L"График зависимости v\'(x)";
			this->Chart2->Titles->Add(title2);
			// 
			// Chart3
			// 
			chartArea3->Name = L"ChartArea1";
			this->Chart3->ChartAreas->Add(chartArea3);
			legend3->Name = L"Legend1";
			this->Chart3->Legends->Add(legend3);
			this->Chart3->Location = System::Drawing::Point(672, 520);
			this->Chart3->Name = L"Chart3";
			series5->ChartArea = L"ChartArea1";
			series5->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series5->Legend = L"Legend1";
			series5->Name = L"Истинное решение";
			series6->ChartArea = L"ChartArea1";
			series6->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series6->Legend = L"Legend1";
			series6->Name = L"Численное решение";
			this->Chart3->Series->Add(series5);
			this->Chart3->Series->Add(series6);
			this->Chart3->Size = System::Drawing::Size(300, 300);
			this->Chart3->TabIndex = 11;
			this->Chart3->Text = L"Chart3";
			title3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			title3->Name = L"Title1";
			title3->Text = L"График зависимости v\'(v)";
			this->Chart3->Titles->Add(title3);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1661, 936);
			this->Controls->Add(this->Chart3);
			this->Controls->Add(this->Chart2);
			this->Controls->Add(this->Chart1);
			this->Controls->Add(this->ControlBox);
			this->Controls->Add(this->ParamMethodBox);
			this->Controls->Add(this->NUBox);
			this->Controls->Add(this->ParamSystBox);
			this->Controls->Add(this->MenuStrip);
			this->MainMenuStrip = this->MenuStrip;
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"MainForm";
			this->MenuStrip->ResumeLayout(false);
			this->MenuStrip->PerformLayout();
			this->ParamSystBox->ResumeLayout(false);
			this->ParamSystBox->PerformLayout();
			this->NUBox->ResumeLayout(false);
			this->NUBox->PerformLayout();
			this->ParamMethodBox->ResumeLayout(false);
			this->ParamMethodBox->PerformLayout();
			this->ControlBox->ResumeLayout(false);
			this->ControlBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Chart1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Chart2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Chart3))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void StartMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		if (!Double::TryParse(mTextBox->Text, System::Globalization::NumberStyles::Float, System::Globalization::CultureInfo::InvariantCulture, m)) {
			MessageBox::Show("m не число", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		else if (m <= 0.0) {
			MessageBox::Show("m меньше либо равно нулю", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		if (!Double::TryParse(kTextBox->Text, System::Globalization::NumberStyles::Float, System::Globalization::CultureInfo::InvariantCulture, k)) {
			MessageBox::Show("k не число", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		else if (k <= 0.0) {
			MessageBox::Show("k меньше либо равно нулю", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		if (!Double::TryParse(fTextBox->Text, System::Globalization::NumberStyles::Float, System::Globalization::CultureInfo::InvariantCulture, f)) {
			MessageBox::Show("f не число", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		else if (f <= 0.0) {
			MessageBox::Show(L"f меньше либо равно нулю", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}

		if (!Double::TryParse(X0TextBox->Text, System::Globalization::NumberStyles::Float, System::Globalization::CultureInfo::InvariantCulture, x0)) {
			MessageBox::Show(L"x₀ не число", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		if (!Double::TryParse(U0TextBox->Text, System::Globalization::NumberStyles::Float, System::Globalization::CultureInfo::InvariantCulture, u0)) {
			MessageBox::Show(L"u₀ не число", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		if (!Double::TryParse(Ud0TextBox->Text, System::Globalization::NumberStyles::Float, System::Globalization::CultureInfo::InvariantCulture, ud0)) {
			MessageBox::Show(L"u'₀ не число", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}

		if (!Double::TryParse(h0TextBox->Text, System::Globalization::NumberStyles::Float, System::Globalization::CultureInfo::InvariantCulture, h0)) {
			MessageBox::Show(L"h₀ не число", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		else if (h0 <= 0.0) {
			MessageBox::Show(L"h₀ меньше либо равно нулю", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}

		if (!Int32::TryParse(NmaxTextBox->Text, System::Globalization::NumberStyles::Float, System::Globalization::CultureInfo::InvariantCulture, Nmax)) {
			MessageBox::Show(L"Nₘₐₓ не число(целое)", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		else if (Nmax <= 0) {
			MessageBox::Show(L"Nₘₐₓ меньше либо равно нулю", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		if (!Double::TryParse(BTextBox->Text, System::Globalization::NumberStyles::Float, System::Globalization::CultureInfo::InvariantCulture, b)) {
			MessageBox::Show("b не число", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		else if (b <= x0) {
			MessageBox::Show("b меньше либо равно x0", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		if (!Double::TryParse(EgrTextBox->Text, System::Globalization::NumberStyles::Float, System::Globalization::CultureInfo::InvariantCulture, Egr)) {
			MessageBox::Show("Eгр не число", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		else if (Egr <= 0.0) {
			MessageBox::Show("Eгр меньше либо равно нулю", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		if (!Double::TryParse(ETextBox->Text, System::Globalization::NumberStyles::Float, System::Globalization::CultureInfo::InvariantCulture, E)) {
			MessageBox::Show("E не число", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		else if (E <= 0.0) {
			MessageBox::Show("E меньше либо равно нулю", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		if (!Double::TryParse(EminTextBox->Text, System::Globalization::NumberStyles::Float, System::Globalization::CultureInfo::InvariantCulture, Emin)) {
			MessageBox::Show(L"Eₘᵢₙ не число", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		else if (Emin <= 0.0) {
			MessageBox::Show(L"Eₘᵢₙ меньше либо равно нулю", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}

		control = (CONTROL)ControlComboBox->SelectedIndex;
		vnext = (VNEXT)VnComboBox->SelectedIndex;

		rkm->setSC(x0, u0, ud0);
		rkm->setH0(h0);
		rkm->setControl(Nmax, b, Egr, E, Emin, control, vnext);
		rkm->solve(*X, *H, *V, *V_cap, *OLP_Arr, *V_res, *step_dec, *step_inc);
		int N = X->size();

		U->resize(2);
		(*U)[0].resize(N);
		(*U)[1].resize(N);
		U_V->resize(N);
		for (int i = 0; i < N; i++) {
			(*U)[0][i] = f1((*X)[i]);
			(*U)[1][i] = f2((*X)[i]);
		}

		calc_C1();
		calc_C2();
		// Рисование графиков
		for (double i = x0; i < b + 4.0; i += 0.001) {
			if (!Double::IsNaN(f1(i)) && !Double::IsNaN(f2(i)) && double_in_dec(f1(i)) && double_in_dec(f2(i))) {
				Chart1->Series["Истинное решение"]->Points->AddXY(i, f1(i));
				Chart2->Series["Истинное решение"]->Points->AddXY(i, f2(i));
				Chart3->Series["Истинное решение"]->Points->AddXY(f1(i), f2(i));
			}
		}
		for (int i = 0; i < N; i++) {
			if (!Double::IsNaN((*V)[0][i]) && !Double::IsNaN((*V)[1][i]) && double_in_dec((*V)[0][i]) && double_in_dec((*V)[1][i])) {
				Chart1->Series["Численное решение"]->Points->AddXY((*X)[i], (*V)[0][i]);
				Chart2->Series["Численное решение"]->Points->AddXY((*X)[i], (*V)[1][i]);
				Chart3->Series["Численное решение"]->Points->AddXY((*V)[0][i], (*V)[1][i]);
			}
		}

		//Заполнение таблицы
		if (Table->Rows->Count < N) {
			int k = N + 1 - Table->Rows->Count;
			for (int i = 0; i < k; i++) {
				Table->Rows->Add();
			}
		}
		else if (Table->Rows->Count > 2 * N) {
			int k = Table->Rows->Count - 1;
			for (int i = k; i > N; i--) {
				Table->Rows->RemoveAt(i);
			}
		}
		Table->Rows[0]->Cells[0]->Value = "0";
		Table->Rows[0]->Cells[2]->Value = (*X)[0].ToString();
		Table->Rows[0]->Cells[3]->Value = (*V)[0][0].ToString();
		Table->Rows[0]->Cells[4]->Value = (*V)[1][0].ToString();
		//	Table->Rows[0]->Cells[2]->Value = V[0].ToString();
		for (int i = 1; i < N; i++) {
			Table->Rows[i]->Cells[0]->Value = i.ToString();
			Table->Rows[i]->Cells[1]->Value = (*H)[i - 1].ToString();
			Table->Rows[i]->Cells[2]->Value = (*X)[i].ToString();
			Table->Rows[i]->Cells[3]->Value = (*V)[0][i].ToString();
			Table->Rows[i]->Cells[4]->Value = (*V)[1][i].ToString();
			Table->Rows[i]->Cells[5]->Value = (*V_cap)[0][i - 1].ToString();
			Table->Rows[i]->Cells[6]->Value = (*V_cap)[1][i - 1].ToString();
			Table->Rows[i]->Cells[7]->Value = (*OLP_Arr)[i - 1].ToString();
			Table->Rows[i]->Cells[8]->Value = (*V_res)[0][i - 1].ToString();
			Table->Rows[i]->Cells[9]->Value = (*V_res)[1][i - 1].ToString();
			//Table->Rows[i]->Cells[10]->Value = X[i].ToString();
			//Table->Rows[i]->Cells[11]->Value = V[i].ToString();
			//Table->Rows[i]->Cells[12]->Value = V2[i].ToString();
			Table->Rows[i]->Cells[13]->Value = (*step_dec)[i - 1].ToString();
			Table->Rows[i]->Cells[14]->Value = (*step_inc)[i - 1].ToString();
		}
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
