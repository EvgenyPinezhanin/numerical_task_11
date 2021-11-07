#pragma once
#define _USE_MATH_DEFINES
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

	// Структура для хранения информации об испытании
	struct Info {
		VNEXT vnext;
		double m, k, f, f_st;
		double x0, u0, ud0;
		double b, Egr;
		double h0;
		int Nmax;
		double E, Emin;
		CONTROL control;
		int N;
		double b_xn;
		double xn, vn_res, vdn_res;
		double maxE, xn_maxE;
		double maxS, xn_maxS;
		double minS, xn_minS;
		double count_dec, count_inc;
		double maxH, xn_1_maxH;
		double minH, xn_1_minH;
	
		Info(VNEXT _vnext, double _m, double _k, double _f, double _f_st, double _x0, double _u0, double _ud0, double _b, double _Egr, 
			double _h0, int _Nmax, double _E, double _Emin, CONTROL _control, int _N, double _b_xn, double _xn, double _vn_res, double _vdn_res,
			double _maxE, double _xn_maxE, double _maxS, double _xn_maxS, double _minS, double _xn_minS, double _count_dec, double _count_inc,
			double _maxH, double _xn_1_maxH, double _minH, double _xn_1_minH) : vnext(_vnext), m(_m), k(_k), f(_f), f_st(_f_st), x0(_x0), 
			u0(_u0), ud0(_ud0), b(_b), Egr(_Egr), h0(_h0), Nmax(_Nmax), E(_E), Emin(_Emin), control(_control), N(_N), b_xn(_b_xn), xn(_xn), 
			vn_res(_vn_res), vdn_res(_vdn_res), maxE(_maxE), xn_maxE(_xn_maxE), maxS(_maxS), xn_maxS(_xn_maxS), minS(_minS), xn_minS(_xn_minS),
			count_dec(_count_dec), count_inc(_count_inc), maxH(_maxH), xn_1_maxH(_xn_1_maxH), minH(_minH), xn_1_minH(_xn_1_minH) {};
	};

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

	// Поиск индекса максимального элемента в vector
	template<typename T>
	int max_elem_ind(const vector<T>& arr) {
		if (arr.empty()) return -1;
		T m = arr[0];
		int ind = 0;
		for (int i = 1; i < arr.size(); i++) {
			if (arr[i] > m) {
				m = arr[i];
				ind = i;
			}
		}
		return ind;
	}

	// Поиск индекса минимального элемента в vector
	template<typename T>
	int min_elem_ind(const vector<T>& arr) {
		if (arr.empty()) return -1;
		T m = arr[0];
		int ind = 0;
		for (int i = 1; i < arr.size(); i++) {
			if (arr[i] < m) {
				m = arr[i];
				ind = i;
			}
		}
		return ind;
	}

	// Нахождение суммы элементов в vector
	template<typename T>
	int sum_elem(const vector<T>& arr) {
		T sum = T(0);
		for (int i = 0; i < arr.size(); i++) {
			sum += arr[i];
		}
		return sum;
	}

	// Функция signum
	template <typename T> int sgn(T val) {
		return (T(0) < val) - (val < T(0));
	}

	// Параметры задания
	double m, k, f, f_st;
	// Начальные условия
	double x0, u0, ud0;
	// Ускорение свободного падения
	const double g = 9.81;
	// F = f / f*, a - "+" граница области застоя, T - период колебаний
	double F, a, T; 

	// ДУ для u'
	double ud(double x, double u1, double u2) {
		return u2;
	}

	// ДУ для u''
	double udd(double x, double u1, double u2) {
		if (u2 != 0) {
			return -sgn(u2) * f * g - k / m * u1;
		} else {
			if (abs(k * u1) <= f_st * m * g) {
				return 0;
			} else {
				return sgn(k * u1) * f_st * g - k / m * u1;
			}
		}
	}

	// Решение для u(x)
	double f1(double x) {
		int n = (int)(x / (T / 2.0));
		double pi = (u0 < 0) ? M_PI : 0.0;
		double c = cos(sqrt(k / m) * x + pi);
		double sg = sgn(u0);
		if (n % 2 == 0) {
			return sg * a * F + (sg * u0 - (2 * n + 1) * a * F) * c;
		} else {
			return -sg * a * F + (sg * u0 - (2 * n + 1) * a * F) * c;
		}
	}

	// Решение для u'(x)
	double f2(double x) {
		int n = (int)(x / (T / 2.0));
		double pi = (u0 < 0) ? M_PI : 0.0;
		double c = sin(sqrt(k / m) * x + pi);
		return -sqrt(k / m) * (sgn(u0) * u0 - (2 * n + 1) * a * F) * c;
	}

	/// <summary>
	/// Сводка для MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		// Начальный шаг
		double h0;
		// Максимальное количество шагов
		int Nmax;
		// Параметры контроля
		double b, Egr, E, Emin;
		// Тип контроля
		CONTROL control;
		// Способ счета
		VNEXT vnext;

		// Vector's для хранения информации после испытания
		vector<double>* X, * OLP_Arr, * H, * U_V;
		vector<vector<double>>* V, * V_cap, * V_res, * U;
		vector<int>* step_dec, * step_inc;

		// Объект метода
		rkm_method* rkm;

		// Итоговое количество шагов + 1
		size_t N;
		// Vector для хранения справки после испытания
		vector<Info> *info_array;

		// Переменная для нумерования кривых на графиках
		int count_trial;
		
		// Окно с информацией о методе
		MethodForm^ mForm;

		// Окно с информацией о задании
		TaskForm^ tForm;

		MainForm(void)
		{
			InitializeComponent();

			rkm = new rkm_method(ud, true, udd, true);
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

			info_array = new vector<Info>;

			count_trial = 0;

			// Установка двойной буфферизации
			SetDoubleBuffered(Table, true);
			SetDoubleBuffered(Chart1, true);
			SetDoubleBuffered(Chart2, true);
			SetDoubleBuffered(Chart3, true);
			
			// Устанвка всплывающих подсказок
			mToolTip->SetToolTip(mLabel, "Масса груза");
			kToolTip->SetToolTip(kLabel, "Жесткость пружины");
			fToolTip->SetToolTip(fLabel, "Коэффициент пропорциональности силы трения");
			fStToolTip->SetToolTip(FstLabel, "Коэффициент пропорциональности силы трения покоя");
			x0ToolTip->SetToolTip(X0Label, "Начальное значение времени");
			u0ToolTip->SetToolTip(U0Label, "Начальное значение отклонения груза от состояния равновесия");
			ud0ToolTip->SetToolTip(Ud0Label, "Начальное значение скорости груза");
			h0ToolTip->SetToolTip(h0Label, "Начальный шаг");
			NmaxToolTip->SetToolTip(NMaxLabel, "Максимально допустимое число шагов");
			bToolTip->SetToolTip(BLabel, "Правая граница участка интегрирования");
			EgrToolTip->SetToolTip(EgrLabel, "Параметр контроля выхода на правую границу");
			EminToolTip->SetToolTip(EminLabel, "Параметр контроля локальной погрешности \"снизу\"");
			defButToolTip->SetToolTip(DefEminButton, L" Eₘᵢₙ = E / 2^(p + 1), p - порядок метода");

			// Инициализация окон
			mForm = gcnew MethodForm();
			tForm = gcnew TaskForm();
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

#pragma region declaration
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
	private: System::Windows::Forms::Label^ X0Label;
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
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ Chart1;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ Chart2;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ Chart3;
	private: System::Windows::Forms::DataGridView^ Table;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ n;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ hn_1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ xn;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ vn;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ vdn;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ vCapn;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ vCapdn;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ s_star;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ vn_res;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ vdn_res;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ un;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ udn;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ un_vn;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ dec_step;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ inc_step;
	private: System::Windows::Forms::Button^ DefEminButton;
	private: System::Windows::Forms::TextBox^ InfoTextBox;
	private: System::Windows::Forms::ToolStripMenuItem^ ChartMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ AlignChartMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ ClearChartMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ ClearAllChartMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ ClearAll_OneChartMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ InfoMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ ClaerInfoMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ ClearAllInfoMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ ClearAll_OneInfoMenuItem;
	private: System::Windows::Forms::TableLayoutPanel^ TableInfoLayoutPanel;
	private: System::Windows::Forms::TextBox^ fStTextBox;
	private: System::Windows::Forms::Label^ FstLabel;
	private: System::Windows::Forms::SplitContainer^ Chart1Container;
	private: System::Windows::Forms::SplitContainer^ Chart2Container;
	private: System::Windows::Forms::ToolTip^ mToolTip;
	private: System::Windows::Forms::ToolTip^ kToolTip;
	private: System::Windows::Forms::ToolTip^ fToolTip;
	private: System::Windows::Forms::ToolTip^ fStToolTip;
	private: System::Windows::Forms::ToolTip^ x0ToolTip;
	private: System::Windows::Forms::ToolTip^ u0ToolTip;
	private: System::Windows::Forms::ToolTip^ h0ToolTip;
	private: System::Windows::Forms::ToolTip^ NmaxToolTip;
	private: System::Windows::Forms::ToolTip^ EgrToolTip;
	private: System::Windows::Forms::ToolTip^ EToolTip;
	private: System::Windows::Forms::ToolTip^ EminToolTip;
	private: System::Windows::Forms::ToolTip^ defButToolTip;
	private: System::Windows::Forms::ToolTip^ ud0ToolTip;
	private: System::Windows::Forms::ToolTip^ bToolTip;
	private: System::ComponentModel::IContainer^ components;
#pragma endregion

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea4 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Title^ title4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea5 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend5 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Title^ title5 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea6 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend6 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Title^ title6 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
			this->MenuStrip = (gcnew System::Windows::Forms::MenuStrip());
			this->StartMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TaskMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MethodMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ChartMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->AlignChartMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ClearChartMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ClearAllChartMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ClearAll_OneChartMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->InfoMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ClaerInfoMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ClearAllInfoMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ClearAll_OneInfoMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ParamSystBox = (gcnew System::Windows::Forms::GroupBox());
			this->fStTextBox = (gcnew System::Windows::Forms::TextBox());
			this->FstLabel = (gcnew System::Windows::Forms::Label());
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
			this->X0Label = (gcnew System::Windows::Forms::Label());
			this->ParamMethodBox = (gcnew System::Windows::Forms::GroupBox());
			this->h0TextBox = (gcnew System::Windows::Forms::TextBox());
			this->h0Label = (gcnew System::Windows::Forms::Label());
			this->ControlBox = (gcnew System::Windows::Forms::GroupBox());
			this->DefEminButton = (gcnew System::Windows::Forms::Button());
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
			this->Table = (gcnew System::Windows::Forms::DataGridView());
			this->n = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->hn_1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->xn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->vn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->vdn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->vCapn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->vCapdn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->s_star = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->vn_res = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->vdn_res = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->un = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->udn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->un_vn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dec_step = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->inc_step = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->InfoTextBox = (gcnew System::Windows::Forms::TextBox());
			this->TableInfoLayoutPanel = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->Chart1Container = (gcnew System::Windows::Forms::SplitContainer());
			this->Chart2Container = (gcnew System::Windows::Forms::SplitContainer());
			this->mToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->kToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->fToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->fStToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->x0ToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->u0ToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->ud0ToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->h0ToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->NmaxToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->EgrToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->EToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->EminToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->defButToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->bToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->MenuStrip->SuspendLayout();
			this->ParamSystBox->SuspendLayout();
			this->NUBox->SuspendLayout();
			this->ParamMethodBox->SuspendLayout();
			this->ControlBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Chart1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Chart2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Chart3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Table))->BeginInit();
			this->TableInfoLayoutPanel->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Chart1Container))->BeginInit();
			this->Chart1Container->Panel1->SuspendLayout();
			this->Chart1Container->Panel2->SuspendLayout();
			this->Chart1Container->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Chart2Container))->BeginInit();
			this->Chart2Container->Panel1->SuspendLayout();
			this->Chart2Container->Panel2->SuspendLayout();
			this->Chart2Container->SuspendLayout();
			this->SuspendLayout();
			// 
			// MenuStrip
			// 
			this->MenuStrip->ImageScalingSize = System::Drawing::Size(20, 20);
			this->MenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->StartMenuItem, this->TaskMenuItem,
					this->MethodMenuItem, this->ChartMenuItem, this->InfoMenuItem
			});
			this->MenuStrip->Location = System::Drawing::Point(0, 0);
			this->MenuStrip->Name = L"MenuStrip";
			this->MenuStrip->Size = System::Drawing::Size(1899, 28);
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
			// ChartMenuItem
			// 
			this->ChartMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->AlignChartMenuItem,
					this->ClearChartMenuItem
			});
			this->ChartMenuItem->Name = L"ChartMenuItem";
			this->ChartMenuItem->Size = System::Drawing::Size(73, 24);
			this->ChartMenuItem->Text = L"График";
			// 
			// AlignChartMenuItem
			// 
			this->AlignChartMenuItem->Name = L"AlignChartMenuItem";
			this->AlignChartMenuItem->Size = System::Drawing::Size(224, 26);
			this->AlignChartMenuItem->Text = L"Выровнять";
			this->AlignChartMenuItem->Click += gcnew System::EventHandler(this, &MainForm::AlignChartMenuItem_Click);
			// 
			// ClearChartMenuItem
			// 
			this->ClearChartMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->ClearAllChartMenuItem,
					this->ClearAll_OneChartMenuItem
			});
			this->ClearChartMenuItem->Name = L"ClearChartMenuItem";
			this->ClearChartMenuItem->Size = System::Drawing::Size(224, 26);
			this->ClearChartMenuItem->Text = L"Очистить график";
			// 
			// ClearAllChartMenuItem
			// 
			this->ClearAllChartMenuItem->Name = L"ClearAllChartMenuItem";
			this->ClearAllChartMenuItem->Size = System::Drawing::Size(389, 26);
			this->ClearAllChartMenuItem->Text = L"Удалить все траектории";
			this->ClearAllChartMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ClearAllChartMenuItem_Click);
			// 
			// ClearAll_OneChartMenuItem
			// 
			this->ClearAll_OneChartMenuItem->Name = L"ClearAll_OneChartMenuItem";
			this->ClearAll_OneChartMenuItem->Size = System::Drawing::Size(389, 26);
			this->ClearAll_OneChartMenuItem->Text = L"Удалить все траектории, кроме последней";
			this->ClearAll_OneChartMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ClearAll_OneChartMenuItem_Click);
			// 
			// InfoMenuItem
			// 
			this->InfoMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->ClaerInfoMenuItem });
			this->InfoMenuItem->Name = L"InfoMenuItem";
			this->InfoMenuItem->Size = System::Drawing::Size(81, 24);
			this->InfoMenuItem->Text = L"Справка";
			// 
			// ClaerInfoMenuItem
			// 
			this->ClaerInfoMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->ClearAllInfoMenuItem,
					this->ClearAll_OneInfoMenuItem
			});
			this->ClaerInfoMenuItem->Name = L"ClaerInfoMenuItem";
			this->ClaerInfoMenuItem->Size = System::Drawing::Size(215, 26);
			this->ClaerInfoMenuItem->Text = L"Очистить справку";
			// 
			// ClearAllInfoMenuItem
			// 
			this->ClearAllInfoMenuItem->Name = L"ClearAllInfoMenuItem";
			this->ClearAllInfoMenuItem->Size = System::Drawing::Size(454, 26);
			this->ClearAllInfoMenuItem->Text = L"Удалить данные всех испытаний";
			this->ClearAllInfoMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ClearAllInfoMenuItem_Click);
			// 
			// ClearAll_OneInfoMenuItem
			// 
			this->ClearAll_OneInfoMenuItem->Name = L"ClearAll_OneInfoMenuItem";
			this->ClearAll_OneInfoMenuItem->Size = System::Drawing::Size(454, 26);
			this->ClearAll_OneInfoMenuItem->Text = L"Удалить данные всех испитаний, кроме последнего";
			this->ClearAll_OneInfoMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ClearAll_OneInfoMenuItem_Click);
			// 
			// ParamSystBox
			// 
			this->ParamSystBox->Controls->Add(this->fStTextBox);
			this->ParamSystBox->Controls->Add(this->FstLabel);
			this->ParamSystBox->Controls->Add(this->fTextBox);
			this->ParamSystBox->Controls->Add(this->kTextBox);
			this->ParamSystBox->Controls->Add(this->mTextBox);
			this->ParamSystBox->Controls->Add(this->fLabel);
			this->ParamSystBox->Controls->Add(this->kLabel);
			this->ParamSystBox->Controls->Add(this->mLabel);
			this->ParamSystBox->Location = System::Drawing::Point(12, 31);
			this->ParamSystBox->Name = L"ParamSystBox";
			this->ParamSystBox->Size = System::Drawing::Size(179, 138);
			this->ParamSystBox->TabIndex = 4;
			this->ParamSystBox->TabStop = false;
			this->ParamSystBox->Text = L"Параметры системы";
			// 
			// fStTextBox
			// 
			this->fStTextBox->Location = System::Drawing::Point(31, 105);
			this->fStTextBox->Name = L"fStTextBox";
			this->fStTextBox->Size = System::Drawing::Size(134, 22);
			this->fStTextBox->TabIndex = 7;
			this->fStTextBox->Text = L"0.4";
			// 
			// FstLabel
			// 
			this->FstLabel->AutoSize = true;
			this->FstLabel->Location = System::Drawing::Point(7, 108);
			this->FstLabel->Name = L"FstLabel";
			this->FstLabel->Size = System::Drawing::Size(17, 17);
			this->FstLabel->TabIndex = 6;
			this->FstLabel->Text = L"f*";
			// 
			// fTextBox
			// 
			this->fTextBox->Location = System::Drawing::Point(31, 77);
			this->fTextBox->Name = L"fTextBox";
			this->fTextBox->Size = System::Drawing::Size(134, 22);
			this->fTextBox->TabIndex = 5;
			this->fTextBox->Text = L"0.3";
			// 
			// kTextBox
			// 
			this->kTextBox->Location = System::Drawing::Point(31, 49);
			this->kTextBox->Name = L"kTextBox";
			this->kTextBox->Size = System::Drawing::Size(134, 22);
			this->kTextBox->TabIndex = 4;
			this->kTextBox->Text = L"175";
			// 
			// mTextBox
			// 
			this->mTextBox->Location = System::Drawing::Point(31, 21);
			this->mTextBox->Name = L"mTextBox";
			this->mTextBox->Size = System::Drawing::Size(134, 22);
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
			this->NUBox->Controls->Add(this->X0Label);
			this->NUBox->Location = System::Drawing::Point(197, 31);
			this->NUBox->Name = L"NUBox";
			this->NUBox->Size = System::Drawing::Size(200, 138);
			this->NUBox->TabIndex = 5;
			this->NUBox->TabStop = false;
			this->NUBox->Text = L"Начальные условия";
			// 
			// Ud0TextBox
			// 
			this->Ud0TextBox->Location = System::Drawing::Point(31, 75);
			this->Ud0TextBox->Name = L"Ud0TextBox";
			this->Ud0TextBox->Size = System::Drawing::Size(163, 22);
			this->Ud0TextBox->TabIndex = 5;
			this->Ud0TextBox->Text = L"0";
			// 
			// U0TextBox
			// 
			this->U0TextBox->Location = System::Drawing::Point(31, 47);
			this->U0TextBox->Name = L"U0TextBox";
			this->U0TextBox->Size = System::Drawing::Size(163, 22);
			this->U0TextBox->TabIndex = 4;
			this->U0TextBox->Text = L"7.5";
			// 
			// X0TextBox
			// 
			this->X0TextBox->Location = System::Drawing::Point(31, 19);
			this->X0TextBox->Name = L"X0TextBox";
			this->X0TextBox->Size = System::Drawing::Size(163, 22);
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
			// X0Label
			// 
			this->X0Label->AutoSize = true;
			this->X0Label->Location = System::Drawing::Point(6, 22);
			this->X0Label->Name = L"X0Label";
			this->X0Label->Size = System::Drawing::Size(19, 17);
			this->X0Label->TabIndex = 0;
			this->X0Label->Text = L"x₀";
			// 
			// ParamMethodBox
			// 
			this->ParamMethodBox->Controls->Add(this->h0TextBox);
			this->ParamMethodBox->Controls->Add(this->h0Label);
			this->ParamMethodBox->Location = System::Drawing::Point(12, 175);
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
			this->h0TextBox->Size = System::Drawing::Size(331, 22);
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
			this->ControlBox->Controls->Add(this->DefEminButton);
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
			this->ControlBox->Location = System::Drawing::Point(12, 238);
			this->ControlBox->Name = L"ControlBox";
			this->ControlBox->Size = System::Drawing::Size(385, 227);
			this->ControlBox->TabIndex = 8;
			this->ControlBox->TabStop = false;
			this->ControlBox->Text = L"Контроль";
			// 
			// DefEminButton
			// 
			this->DefEminButton->Location = System::Drawing::Point(291, 132);
			this->DefEminButton->Name = L"DefEminButton";
			this->DefEminButton->Size = System::Drawing::Size(88, 24);
			this->DefEminButton->TabIndex = 12;
			this->DefEminButton->Text = L"По умолч.";
			this->DefEminButton->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			this->DefEminButton->UseVisualStyleBackColor = true;
			this->DefEminButton->Click += gcnew System::EventHandler(this, &MainForm::DefEminButton_Click);
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
			this->VnComboBox->Size = System::Drawing::Size(370, 24);
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
			this->ControlComboBox->Size = System::Drawing::Size(370, 24);
			this->ControlComboBox->TabIndex = 10;
			this->ControlComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::ControlComboBox_SelectedIndexChanged);
			// 
			// EminTextBox
			// 
			this->EminTextBox->Location = System::Drawing::Point(48, 133);
			this->EminTextBox->Name = L"EminTextBox";
			this->EminTextBox->Size = System::Drawing::Size(237, 22);
			this->EminTextBox->TabIndex = 9;
			this->EminTextBox->Text = L"0.0000005";
			// 
			// ETextBox
			// 
			this->ETextBox->Location = System::Drawing::Point(48, 105);
			this->ETextBox->Name = L"ETextBox";
			this->ETextBox->Size = System::Drawing::Size(331, 22);
			this->ETextBox->TabIndex = 8;
			this->ETextBox->Text = L"0.000005";
			// 
			// EgrTextBox
			// 
			this->EgrTextBox->Location = System::Drawing::Point(48, 77);
			this->EgrTextBox->Name = L"EgrTextBox";
			this->EgrTextBox->Size = System::Drawing::Size(331, 22);
			this->EgrTextBox->TabIndex = 7;
			this->EgrTextBox->Text = L"0.000005";
			// 
			// BTextBox
			// 
			this->BTextBox->Location = System::Drawing::Point(48, 49);
			this->BTextBox->Name = L"BTextBox";
			this->BTextBox->Size = System::Drawing::Size(331, 22);
			this->BTextBox->TabIndex = 6;
			this->BTextBox->Text = L"2.0";
			// 
			// NmaxTextBox
			// 
			this->NmaxTextBox->Location = System::Drawing::Point(48, 21);
			this->NmaxTextBox->Name = L"NmaxTextBox";
			this->NmaxTextBox->Size = System::Drawing::Size(331, 22);
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
			chartArea4->Name = L"ChartArea1";
			this->Chart1->ChartAreas->Add(chartArea4);
			this->Chart1->Dock = System::Windows::Forms::DockStyle::Fill;
			legend4->Name = L"Legend1";
			this->Chart1->Legends->Add(legend4);
			this->Chart1->Location = System::Drawing::Point(0, 0);
			this->Chart1->Name = L"Chart1";
			this->Chart1->Size = System::Drawing::Size(629, 474);
			this->Chart1->TabIndex = 9;
			this->Chart1->Text = L"Chart1";
			title4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			title4->Name = L"Title1";
			title4->Text = L"График зависимости v(x)";
			this->Chart1->Titles->Add(title4);
			// 
			// Chart2
			// 
			chartArea5->Name = L"ChartArea1";
			this->Chart2->ChartAreas->Add(chartArea5);
			this->Chart2->Dock = System::Windows::Forms::DockStyle::Fill;
			legend5->Name = L"Legend1";
			this->Chart2->Legends->Add(legend5);
			this->Chart2->Location = System::Drawing::Point(0, 0);
			this->Chart2->Name = L"Chart2";
			this->Chart2->Size = System::Drawing::Size(625, 474);
			this->Chart2->TabIndex = 10;
			this->Chart2->Text = L"Chart2";
			title5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			title5->Name = L"Title1";
			title5->Text = L"График зависимости v\'(x)";
			this->Chart2->Titles->Add(title5);
			// 
			// Chart3
			// 
			chartArea6->Name = L"ChartArea1";
			this->Chart3->ChartAreas->Add(chartArea6);
			this->Chart3->Dock = System::Windows::Forms::DockStyle::Fill;
			legend6->Name = L"Legend1";
			this->Chart3->Legends->Add(legend6);
			this->Chart3->Location = System::Drawing::Point(0, 0);
			this->Chart3->Name = L"Chart3";
			this->Chart3->Size = System::Drawing::Size(625, 474);
			this->Chart3->TabIndex = 11;
			this->Chart3->Text = L"Chart3";
			title6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			title6->Name = L"Title1";
			title6->Text = L"График зависимости v\'(v)";
			this->Chart3->Titles->Add(title6);
			// 
			// Table
			// 
			this->Table->AllowUserToAddRows = false;
			this->Table->AllowUserToDeleteRows = false;
			this->Table->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->Table->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(15) {
				this->n, this->hn_1,
					this->xn, this->vn, this->vdn, this->vCapn, this->vCapdn, this->s_star, this->vn_res, this->vdn_res, this->un, this->udn, this->un_vn,
					this->dec_step, this->inc_step
			});
			this->Table->Dock = System::Windows::Forms::DockStyle::Fill;
			this->Table->Location = System::Drawing::Point(3, 3);
			this->Table->Name = L"Table";
			this->Table->ReadOnly = true;
			this->Table->RowHeadersWidth = 51;
			this->Table->RowTemplate->Height = 24;
			this->Table->Size = System::Drawing::Size(929, 427);
			this->Table->TabIndex = 12;
			// 
			// n
			// 
			this->n->HeaderText = L"n";
			this->n->MinimumWidth = 6;
			this->n->Name = L"n";
			this->n->ReadOnly = true;
			this->n->Width = 125;
			// 
			// hn_1
			// 
			this->hn_1->HeaderText = L"hₙ₋₁";
			this->hn_1->MinimumWidth = 6;
			this->hn_1->Name = L"hn_1";
			this->hn_1->ReadOnly = true;
			this->hn_1->Width = 125;
			// 
			// xn
			// 
			this->xn->HeaderText = L"xₙ";
			this->xn->MinimumWidth = 6;
			this->xn->Name = L"xn";
			this->xn->ReadOnly = true;
			this->xn->Width = 125;
			// 
			// vn
			// 
			this->vn->HeaderText = L"vₙ";
			this->vn->MinimumWidth = 6;
			this->vn->Name = L"vn";
			this->vn->ReadOnly = true;
			this->vn->Width = 125;
			// 
			// vdn
			// 
			this->vdn->HeaderText = L"v\'ₙ";
			this->vdn->MinimumWidth = 6;
			this->vdn->Name = L"vdn";
			this->vdn->ReadOnly = true;
			this->vdn->Width = 125;
			// 
			// vCapn
			// 
			this->vCapn->HeaderText = L"v̂ₙ";
			this->vCapn->MinimumWidth = 6;
			this->vCapn->Name = L"vCapn";
			this->vCapn->ReadOnly = true;
			this->vCapn->Width = 125;
			// 
			// vCapdn
			// 
			this->vCapdn->HeaderText = L"v̂\'ₙ";
			this->vCapdn->MinimumWidth = 6;
			this->vCapdn->Name = L"vCapdn";
			this->vCapdn->ReadOnly = true;
			this->vCapdn->Width = 125;
			// 
			// s_star
			// 
			this->s_star->HeaderText = L"S*";
			this->s_star->MinimumWidth = 6;
			this->s_star->Name = L"s_star";
			this->s_star->ReadOnly = true;
			this->s_star->Width = 125;
			// 
			// vn_res
			// 
			this->vn_res->HeaderText = L"vₙ итог";
			this->vn_res->MinimumWidth = 6;
			this->vn_res->Name = L"vn_res";
			this->vn_res->ReadOnly = true;
			this->vn_res->Width = 125;
			// 
			// vdn_res
			// 
			this->vdn_res->HeaderText = L"v\'ₙ итог";
			this->vdn_res->MinimumWidth = 6;
			this->vdn_res->Name = L"vdn_res";
			this->vdn_res->ReadOnly = true;
			this->vdn_res->Width = 125;
			// 
			// un
			// 
			this->un->HeaderText = L"uₙ";
			this->un->MinimumWidth = 6;
			this->un->Name = L"un";
			this->un->ReadOnly = true;
			this->un->Width = 125;
			// 
			// udn
			// 
			this->udn->HeaderText = L"u\'ₙ";
			this->udn->MinimumWidth = 6;
			this->udn->Name = L"udn";
			this->udn->ReadOnly = true;
			this->udn->Width = 125;
			// 
			// un_vn
			// 
			this->un_vn->HeaderText = L"|Uₙ-Vₙ итог|";
			this->un_vn->MinimumWidth = 6;
			this->un_vn->Name = L"un_vn";
			this->un_vn->ReadOnly = true;
			this->un_vn->Width = 125;
			// 
			// dec_step
			// 
			this->dec_step->HeaderText = L"Ум. шага";
			this->dec_step->MinimumWidth = 6;
			this->dec_step->Name = L"dec_step";
			this->dec_step->ReadOnly = true;
			this->dec_step->Width = 125;
			// 
			// inc_step
			// 
			this->inc_step->HeaderText = L"Ув. шага";
			this->inc_step->MinimumWidth = 6;
			this->inc_step->Name = L"inc_step";
			this->inc_step->ReadOnly = true;
			this->inc_step->Width = 125;
			// 
			// InfoTextBox
			// 
			this->InfoTextBox->AcceptsReturn = true;
			this->InfoTextBox->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->InfoTextBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->InfoTextBox->ForeColor = System::Drawing::SystemColors::WindowText;
			this->InfoTextBox->Location = System::Drawing::Point(938, 3);
			this->InfoTextBox->Multiline = true;
			this->InfoTextBox->Name = L"InfoTextBox";
			this->InfoTextBox->ReadOnly = true;
			this->InfoTextBox->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->InfoTextBox->Size = System::Drawing::Size(555, 427);
			this->InfoTextBox->TabIndex = 13;
			// 
			// TableInfoLayoutPanel
			// 
			this->TableInfoLayoutPanel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->TableInfoLayoutPanel->ColumnCount = 2;
			this->TableInfoLayoutPanel->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				62.5F)));
			this->TableInfoLayoutPanel->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				37.5F)));
			this->TableInfoLayoutPanel->Controls->Add(this->Table, 0, 0);
			this->TableInfoLayoutPanel->Controls->Add(this->InfoTextBox, 1, 0);
			this->TableInfoLayoutPanel->Location = System::Drawing::Point(403, 32);
			this->TableInfoLayoutPanel->Name = L"TableInfoLayoutPanel";
			this->TableInfoLayoutPanel->RowCount = 1;
			this->TableInfoLayoutPanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent,
				100)));
			this->TableInfoLayoutPanel->Size = System::Drawing::Size(1496, 433);
			this->TableInfoLayoutPanel->TabIndex = 15;
			// 
			// Chart1Container
			// 
			this->Chart1Container->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->Chart1Container->Location = System::Drawing::Point(0, 471);
			this->Chart1Container->Name = L"Chart1Container";
			// 
			// Chart1Container.Panel1
			// 
			this->Chart1Container->Panel1->Controls->Add(this->Chart2Container);
			// 
			// Chart1Container.Panel2
			// 
			this->Chart1Container->Panel2->Controls->Add(this->Chart3);
			this->Chart1Container->Size = System::Drawing::Size(1887, 474);
			this->Chart1Container->SplitterDistance = 1258;
			this->Chart1Container->TabIndex = 16;
			// 
			// Chart2Container
			// 
			this->Chart2Container->Dock = System::Windows::Forms::DockStyle::Fill;
			this->Chart2Container->Location = System::Drawing::Point(0, 0);
			this->Chart2Container->Name = L"Chart2Container";
			// 
			// Chart2Container.Panel1
			// 
			this->Chart2Container->Panel1->Controls->Add(this->Chart1);
			// 
			// Chart2Container.Panel2
			// 
			this->Chart2Container->Panel2->Controls->Add(this->Chart2);
			this->Chart2Container->Size = System::Drawing::Size(1258, 474);
			this->Chart2Container->SplitterDistance = 629;
			this->Chart2Container->TabIndex = 0;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1899, 957);
			this->Controls->Add(this->Chart1Container);
			this->Controls->Add(this->TableInfoLayoutPanel);
			this->Controls->Add(this->ControlBox);
			this->Controls->Add(this->ParamMethodBox);
			this->Controls->Add(this->NUBox);
			this->Controls->Add(this->ParamSystBox);
			this->Controls->Add(this->MenuStrip);
			this->MainMenuStrip = this->MenuStrip;
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Задача №11, вариант №5, Пинежанин Евгений, группа 381903_3";
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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Table))->EndInit();
			this->TableInfoLayoutPanel->ResumeLayout(false);
			this->TableInfoLayoutPanel->PerformLayout();
			this->Chart1Container->Panel1->ResumeLayout(false);
			this->Chart1Container->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Chart1Container))->EndInit();
			this->Chart1Container->ResumeLayout(false);
			this->Chart2Container->Panel1->ResumeLayout(false);
			this->Chart2Container->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Chart2Container))->EndInit();
			this->Chart2Container->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	// Добавление новых линий на графики
	private: System::Void addSeries() {
		count_trial++;
		DataVisualization::Charting::Series^ series_true_1 = (gcnew DataVisualization::Charting::Series());
		DataVisualization::Charting::Series^ series_numeric_1 = (gcnew DataVisualization::Charting::Series());
		series_true_1->ChartArea = L"ChartArea1";
		series_true_1->ChartType = DataVisualization::Charting::SeriesChartType::Line;
		series_true_1->Legend = L"Legend1";
		series_true_1->Name = L"Истинное решение №" + count_trial.ToString();
		series_true_1->BorderWidth = 2;

		series_numeric_1->ChartArea = L"ChartArea1";
		series_numeric_1->ChartType = DataVisualization::Charting::SeriesChartType::Line;
		series_numeric_1->Legend = L"Legend1";
		series_numeric_1->Name = L"Численное решение №" + count_trial.ToString();
		series_numeric_1->BorderWidth = 2;

		this->Chart1->Series->Add(series_true_1);
		this->Chart1->Series->Add(series_numeric_1);

		DataVisualization::Charting::Series^ series_true_2 = (gcnew DataVisualization::Charting::Series());
		DataVisualization::Charting::Series^ series_numeric_2 = (gcnew DataVisualization::Charting::Series());
		series_true_2->ChartArea = L"ChartArea1";
		series_true_2->ChartType = DataVisualization::Charting::SeriesChartType::Line;
		series_true_2->Legend = L"Legend1";
		series_true_2->Name = L"Истинное решение №" + count_trial.ToString();
		series_true_2->BorderWidth = 2;
		

		series_numeric_2->ChartArea = L"ChartArea1";
		series_numeric_2->ChartType = DataVisualization::Charting::SeriesChartType::Line;
		series_numeric_2->Legend = L"Legend1";
		series_numeric_2->Name = L"Численное решение №" + count_trial.ToString();
		series_numeric_2->BorderWidth = 2;

		this->Chart2->Series->Add(series_true_2);
		this->Chart2->Series->Add(series_numeric_2);

		DataVisualization::Charting::Series^ series_true_3 = (gcnew DataVisualization::Charting::Series());
		DataVisualization::Charting::Series^ series_numeric_3 = (gcnew DataVisualization::Charting::Series());
		series_true_3->ChartArea = L"ChartArea1";
		series_true_3->ChartType = DataVisualization::Charting::SeriesChartType::Line;
		series_true_3->Legend = L"Legend1";
		series_true_3->Name = L"Истинное решение №" + count_trial.ToString();
		series_true_3->BorderWidth = 2;

		series_numeric_3->ChartArea = L"ChartArea1";
		series_numeric_3->ChartType = DataVisualization::Charting::SeriesChartType::Line;
		series_numeric_3->Legend = L"Legend1";
		series_numeric_3->Name = L"Численное решение №" + count_trial.ToString();
		series_numeric_3->BorderWidth = 2;

		this->Chart3->Series->Add(series_true_3);
		this->Chart3->Series->Add(series_numeric_3);
	}
	// Вывод справки
	private: System::Void printInfo(Info inf) {
		InfoTextBox->Text += "№ варианта задания: 5" + Environment::NewLine;
		InfoTextBox->Text += "Тип задачи: тестовая" + Environment::NewLine;
		InfoTextBox->Text += L"Метод Рунге Кутта с поправкой Мерсона, p = 4, p̂ = 5" + Environment::NewLine;
		InfoTextBox->Text += L"Способ счета: ";
		if (inf.vnext == VN) {
			InfoTextBox->Text += L"Vₙ₊₁ итог = Vₙ₊₁" + Environment::NewLine;
		}
		else if (inf.vnext == VNCAP) {
			InfoTextBox->Text += L"Vₙ₊₁ итог = V̂ₙ₊₁ " + Environment::NewLine;
		}
		else {
			InfoTextBox->Text += L"Vₙ₊₁ итог = Vₙ₊₁ кор" + Environment::NewLine;
		}
		InfoTextBox->Text += L"Параметры задачи: m = " + inf.m.ToString() + ", k = " + inf.k.ToString() +
			", f = " + inf.f.ToString() + ", f* = " + inf.f_st.ToString() + Environment::NewLine;
		InfoTextBox->Text += L"x₀ = " + inf.x0.ToString() + L", u₀ = " + inf.u0.ToString() + L", u'₀ = " + inf.ud0.ToString() + Environment::NewLine;
		InfoTextBox->Text += L"b = " + inf.b.ToString() + L", Eгр = " + inf.Egr.ToString() + Environment::NewLine;
		InfoTextBox->Text += L"h₀ = " + inf.h0.ToString() + L", Nₘₐₓ = " + inf.Nmax.ToString() + Environment::NewLine;
		InfoTextBox->Text += L"E = " + inf.E.ToString() + L", Eₘᵢₙ = " + inf.Emin.ToString() + Environment::NewLine;
		InfoTextBox->Text += L"Контроль: ";
		if (inf.control == UPDOWN) {
			InfoTextBox->Text += L"включен" + Environment::NewLine;
		}
		else if (inf.control == UP) {
			InfoTextBox->Text += L"включен только сверху " + Environment::NewLine;
		}
		else {
			InfoTextBox->Text += L"отключен" + Environment::NewLine;
		}
		InfoTextBox->Text += L"Результат расчета: " + Environment::NewLine;
		InfoTextBox->Text += L"N = " + inf.N.ToString() + Environment::NewLine;
		InfoTextBox->Text += L"b - xₙ = " + inf.b_xn.ToString() + " (n = N)" + Environment::NewLine;
		InfoTextBox->Text += L"xₙ = " + inf.xn.ToString() + L", vₙ = " + inf.vn_res.ToString() +
			L", v'ₙ = " + inf.vdn_res.ToString() + " (n = N)" + Environment::NewLine;
		InfoTextBox->Text += L"max |Eₙ| = " + inf.maxE.ToString() + L" при xₙ = " + inf.xn_maxE.ToString() + Environment::NewLine;
		InfoTextBox->Text += L"max |S| = " + inf.maxS.ToString() + L" при xₙ = " + inf.xn_maxS.ToString() + Environment::NewLine;
		InfoTextBox->Text += L"min |S| = " + inf.minS.ToString() + L" при xₙ = " + inf.xn_minS.ToString() + Environment::NewLine;
		InfoTextBox->Text += L"Всего ум. шага = " + inf.count_dec.ToString() + Environment::NewLine;
		InfoTextBox->Text += L"Всего ув. шага = " + inf.count_inc.ToString() + Environment::NewLine;
		InfoTextBox->Text += L"max |hₙ| = " + inf.maxH.ToString() + L" при xₙ₊₁ = " + inf.xn_1_maxH.ToString() + Environment::NewLine;
		InfoTextBox->Text += L"min |hₙ| = " + inf.minH.ToString() + L" при xₙ₊₁ = " + inf.xn_1_minH.ToString() + Environment::NewLine;
		InfoTextBox->Text += L"------------------------------------------------------" + Environment::NewLine;
		InfoTextBox->SelectionStart = InfoTextBox->Text->Length;
		InfoTextBox->ScrollToCaret();
	}
	// Запуск испытания
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
		if (!Double::TryParse(fStTextBox->Text, System::Globalization::NumberStyles::Float, System::Globalization::CultureInfo::InvariantCulture, f_st)) {
			MessageBox::Show("f не число", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		else if (f_st < f) {
			MessageBox::Show(L"f* меньше f", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
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

		rkm->setSC(x0, u0, ud0);
		rkm->setH0(h0);
		vnext = (VNEXT)VnComboBox->SelectedIndex;
		control = (CONTROL)ControlComboBox->SelectedIndex;
		if (control != CONST) {
			if (!Double::TryParse(ETextBox->Text, System::Globalization::NumberStyles::Float, System::Globalization::CultureInfo::InvariantCulture, E)) {
				MessageBox::Show("E не число", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			} else if (E <= 0.0) {
				MessageBox::Show("E меньше либо равно нулю", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}
			rkm->setControl(control, vnext, Nmax, b, Egr, E);
			if (control == UPDOWN) {
				if (!Double::TryParse(EminTextBox->Text, System::Globalization::NumberStyles::Float, System::Globalization::CultureInfo::InvariantCulture, Emin)) {
					MessageBox::Show(L"Eₘᵢₙ не число", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
					return;
				} else if (Emin <= 0.0) {
					MessageBox::Show(L"Eₘᵢₙ меньше либо равно нулю", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
					return;
				}
				rkm->setControl(control, vnext, Nmax, b, Egr, E, Emin);
			}
			rkm->solve(*X, *H, *V, *V_cap, *OLP_Arr, *V_res, *step_dec, *step_inc);
		} else {
			rkm->setControl(control, vnext, Nmax, b, Egr);
			rkm->solve(*X, *H, *V, *V_cap);
		}
		N = X->size();

		// Установка параметров необходимых для поиска истинного решения
		F = f / f_st;
		a = f_st * m * g / k;
		T = 2 * M_PI / sqrt(k / m);

		// Поиск истинных решений и разницы между истинным решением и численным
		U->resize(2);
		(*U)[0].resize(N);
		(*U)[1].resize(N);
		U_V->resize(N);
		for (int i = 0; i < N; i++) {
			(*U)[0][i] = f1((*X)[i]);
			(*U)[1][i] = f2((*X)[i]);
			(*U_V)[i] = sqrt(pow((*U)[0][i] - (*V)[0][i], 2) + pow((*U)[1][i] - (*V)[1][i], 2));
		}

		// Рисование графиков
		addSeries();
		double r1, r2;
		for (double i = x0; i < b; i += 0.001) {
			if (!Double::IsNaN(f1(i)) && !Double::IsNaN(f2(i)) && double_in_dec(f1(i)) && double_in_dec(f2(i))) {
				r1 = f1(i); r2 = f2(i);
				Chart1->Series["Истинное решение №" + count_trial.ToString()]->Points->AddXY(i, r1);
				Chart2->Series["Истинное решение №" + count_trial.ToString()]->Points->AddXY(i, r2);
				Chart3->Series["Истинное решение №" + count_trial.ToString()]->Points->AddXY(r1, r2);
			}
		}
		for (int i = 0; i < N; i++) {
			if (!Double::IsNaN((*V)[0][i]) && !Double::IsNaN((*V)[1][i]) && double_in_dec((*V)[0][i]) && double_in_dec((*V)[1][i])) {
				Chart1->Series["Численное решение №" + count_trial.ToString()]->Points->AddXY((*X)[i], (*V)[0][i]);
				Chart2->Series["Численное решение №" + count_trial.ToString()]->Points->AddXY((*X)[i], (*V)[1][i]);
				Chart3->Series["Численное решение №" + count_trial.ToString()]->Points->AddXY((*V)[0][i], (*V)[1][i]);
			}
		}
		
		//Заполнение таблицы
		if (Table->Rows->Count < N) {
			int k = (int)N - Table->Rows->Count;
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
		Table->Rows[0]->Cells[10]->Value = (*U)[0][0].ToString();
		Table->Rows[0]->Cells[11]->Value = (*U)[1][0].ToString();
		Table->Rows[0]->Cells[12]->Value = (*U_V)[0].ToString();
		for (int i = 1; i < N; i++) {
			Table->Rows[i]->Cells[0]->Value = i.ToString();
			Table->Rows[i]->Cells[1]->Value = (*H)[i - 1].ToString();
			Table->Rows[i]->Cells[2]->Value = (*X)[i].ToString();
			Table->Rows[i]->Cells[3]->Value = (*V)[0][i].ToString();
			Table->Rows[i]->Cells[4]->Value = (*V)[1][i].ToString();
			Table->Rows[i]->Cells[5]->Value = (*V_cap)[0][i - 1].ToString();
			Table->Rows[i]->Cells[6]->Value = (*V_cap)[1][i - 1].ToString();
			Table->Rows[i]->Cells[10]->Value = (*U)[0][i].ToString();
			Table->Rows[i]->Cells[11]->Value = (*U)[1][i].ToString();
			Table->Rows[i]->Cells[12]->Value = (*U_V)[i].ToString();
			if (control != CONST) {
				Table->Rows[i]->Cells[7]->Value = (*OLP_Arr)[i - 1].ToString();
				Table->Rows[i]->Cells[8]->Value = (*V_res)[0][i - 1].ToString();
				Table->Rows[i]->Cells[9]->Value = (*V_res)[1][i - 1].ToString();
				Table->Rows[i]->Cells[13]->Value = (*step_dec)[i - 1].ToString();
				Table->Rows[i]->Cells[14]->Value = (*step_inc)[i - 1].ToString();
			}
		}

		// Заполнение справки
		int ind;
		ind = max_elem_ind(*U_V);
		double maxE = (*U_V)[ind];
		double xn_maxE = (*X)[ind];
		ind = max_elem_ind(*OLP_Arr);
		double maxS = (*OLP_Arr)[ind];
		double xn_maxS = (*X)[ind];
		ind = min_elem_ind(*OLP_Arr);
		double minS = (*OLP_Arr)[ind];
		double xn_minS = (*X)[ind];
		int count_dec = sum_elem(*step_dec);
		int count_inc = sum_elem(*step_inc);
		H->pop_back();
		ind = max_elem_ind(*H);
		double maxH = (*H)[ind];
		double xn_1_maxH = (*X)[ind + 1];
		ind = min_elem_ind(*H);
		double minH = (*H)[ind];
		double xn_1_minH = (*X)[ind + 1];
		Info inf(vnext, m, k, f, f_st, x0, u0, ud0, b, Egr, h0, Nmax, E, Emin, control, N - 1,
			b - (*X)[N - 1], (*X)[N - 1], (*V_res)[0][N - 2], (*V_res)[1][N - 2], maxE, xn_maxE, maxS, xn_maxS,
			minS, xn_minS, count_dec, count_inc, maxH, xn_1_maxH, minH, xn_1_minH);
		info_array->push_back(inf);
		printInfo(inf);
	}
	// Вывод задания
	private: System::Void TaskMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		tForm->Show();
	}
	// Вывод информации о методе
	private: System::Void MethodMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		mForm->Show();
	}

	private: System::Void ControlComboBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		if (ControlComboBox->SelectedIndex == 2) {
			EminTextBox->Enabled = false;
			ETextBox->Enabled = false;
		} else if (ControlComboBox->SelectedIndex == 1) {
			EminTextBox->Enabled = false;
		} else {
			EminTextBox->Enabled = true;
			ETextBox->Enabled = true;
		}
	}
	// Вычисление Emin
	private: System::Void DefEminButton_Click(System::Object^ sender, System::EventArgs^ e) {
		if (!Double::TryParse(ETextBox->Text, System::Globalization::NumberStyles::Float, System::Globalization::CultureInfo::InvariantCulture, E)) {
			MessageBox::Show("E не число", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		else if (E <= 0.0) {
			MessageBox::Show("E меньше либо равно нулю", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		EminTextBox->Text = (E / pow(2, rkm->getP() + 1)).ToString("N17", System::Globalization::CultureInfo::InvariantCulture);
	}
	// Выпавнивание графиков
	private: System::Void AlignChartMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		Chart1Container->SplitterDistance = (int)(Chart1Container->Size.Width / 3.0 * 2.0);
		Chart2Container->SplitterDistance = (int)(Chart2Container->Size.Width / 2.0);
	}
	private: System::Void ClearAllInfoMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		info_array->clear();
		InfoTextBox->Clear();
	}
	private: System::Void ClearAll_OneInfoMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		Info inf = info_array->back();
		info_array->clear();
		info_array->push_back(inf);
		InfoTextBox->Clear();
		printInfo(inf);
	}
	private: System::Void ClearAllChartMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		int k = Chart1->Series->Count;
		for (int i = 0; i < k; i++) {
			Chart1->Series->RemoveAt(0);
		}
		k = Chart2->Series->Count;
		for (int i = 0; i < k; i++) {
			Chart2->Series->RemoveAt(0);
		}
		k = Chart3->Series->Count;
		for (int i = 0; i < k; i++) {
			Chart3->Series->RemoveAt(0);
		}
		count_trial = 0;
	}
	private: System::Void ClearAll_OneChartMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		int k = Chart1->Series->Count;
		for (int i = 0; i < k - 2; i++) {
			Chart1->Series->RemoveAt(0);
		}
		k = Chart2->Series->Count;
		for (int i = 0; i < k - 2; i++) {
			Chart2->Series->RemoveAt(0);
		}
		k = Chart3->Series->Count;
		for (int i = 0; i < k - 2; i++) {
			Chart3->Series->RemoveAt(0);
		}
		count_trial = 1;
	}
};
}
