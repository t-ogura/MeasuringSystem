#pragma once

#include "VideoInput\vi_cap.h"
#include "Include/define.h"
#include "VCC\vcc.h"
#include "RevisionTool.h"
#include "turnPTU.h"
#include "Pan-tilter_manual.h"

namespace MeasuringSystem {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MainWindow の概要
	/// </summary>
	public ref class MainWindow : public System::Windows::Forms::Form
	{
	public:
		MainWindow(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクター コードを追加します
			//
			readInitFile("initialize.cfg");
			initialize();
			viInit();
			this->Tracking_background->RunWorkerAsync();
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~MainWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Timer^  mainCount;
	private: System::Windows::Forms::PictureBox^  leftImageBox;
	private: System::ComponentModel::BackgroundWorker^  VCC_background;


	private: System::Windows::Forms::PictureBox^  rightImageBox;
	private: System::Windows::Forms::PictureBox^  centerImageBox;
	private: System::Windows::Forms::Label^  OriginalLabel;
	private: System::Windows::Forms::GroupBox^  DistanceGroupBox;
	private: System::Windows::Forms::TableLayoutPanel^  DistancesTable;
	private: System::Windows::Forms::Label^  RevisedLabel;

	private: System::Windows::Forms::Label^  KalmanLabel;
	private: System::ComponentModel::BackgroundWorker^  Tracking_background;
	public: System::Windows::Forms::Label^  OriginalValue;
	private: System::Windows::Forms::GroupBox^  RevisionGroup;


	private: System::Windows::Forms::Button^  RevisionToolButton;
	private: System::Windows::Forms::GroupBox^  ThresholdsGroup;
	private: System::Windows::Forms::TextBox^  AllSearchBox;
	private: System::Windows::Forms::Label^  AllSearchLabel;
	private: System::Windows::Forms::TextBox^  ReliabilityBox;
	private: System::Windows::Forms::Label^  ReliabilityLabel;
	private: System::Windows::Forms::TextBox^  TrackingThresholdBox;
	private: System::Windows::Forms::Label^  TrackingThresholdLabel;
	private: System::Windows::Forms::GroupBox^  TransmitGroup;
	private: System::Windows::Forms::CheckBox^  SendCheck;
	private: System::Windows::Forms::Label^  SystemIDLabel;

	private: System::Windows::Forms::RadioButton^  RadioR;
	private: System::Windows::Forms::RadioButton^  RadioL;
	private: System::Windows::Forms::TextBox^  ServerDirectryBox;
	private: System::Windows::Forms::Button^  FileDirectryReadButton;
	private: System::Windows::Forms::GroupBox^  DataOutputGroup;
	public: System::Windows::Forms::Label^  KalmanValue;
	private:
	public: System::Windows::Forms::Label^  RevisedValue;
	private: System::Windows::Forms::GroupBox^  PanTilterGroupBox;
	private: System::Windows::Forms::CheckBox^  PanTilterEnableCheck;
	private: System::Windows::Forms::Button^  PanTilterManualButton;
	private: System::Windows::Forms::GroupBox^  AnglesBox;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	public: System::Windows::Forms::Label^  PanAngle;
	private:
	private: System::Windows::Forms::Label^  PanAngleLabel;
	public:
	private: System::Windows::Forms::Label^  TiltAngleLabel;
	public: System::Windows::Forms::Label^  TiltAngle;
	private: System::Windows::Forms::Timer^  UIView;
	private: System::Windows::Forms::Button^  SaveStartButton;
	private: System::Windows::Forms::TextBox^  FileNameBox;

	private: System::Windows::Forms::Label^  FileNameLabel;
	private: System::Windows::Forms::NumericUpDown^  FrameNumber;
	private: System::Windows::Forms::Label^  FrameNumLabel;

	public:
	private:
	public:






	protected:
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainWindow::typeid));
			this->mainCount = (gcnew System::Windows::Forms::Timer(this->components));
			this->leftImageBox = (gcnew System::Windows::Forms::PictureBox());
			this->VCC_background = (gcnew System::ComponentModel::BackgroundWorker());
			this->rightImageBox = (gcnew System::Windows::Forms::PictureBox());
			this->centerImageBox = (gcnew System::Windows::Forms::PictureBox());
			this->OriginalLabel = (gcnew System::Windows::Forms::Label());
			this->DistanceGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->DistancesTable = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->KalmanValue = (gcnew System::Windows::Forms::Label());
			this->OriginalValue = (gcnew System::Windows::Forms::Label());
			this->RevisedLabel = (gcnew System::Windows::Forms::Label());
			this->KalmanLabel = (gcnew System::Windows::Forms::Label());
			this->RevisedValue = (gcnew System::Windows::Forms::Label());
			this->Tracking_background = (gcnew System::ComponentModel::BackgroundWorker());
			this->RevisionGroup = (gcnew System::Windows::Forms::GroupBox());
			this->RevisionToolButton = (gcnew System::Windows::Forms::Button());
			this->ThresholdsGroup = (gcnew System::Windows::Forms::GroupBox());
			this->ReliabilityBox = (gcnew System::Windows::Forms::TextBox());
			this->ReliabilityLabel = (gcnew System::Windows::Forms::Label());
			this->TrackingThresholdBox = (gcnew System::Windows::Forms::TextBox());
			this->TrackingThresholdLabel = (gcnew System::Windows::Forms::Label());
			this->AllSearchBox = (gcnew System::Windows::Forms::TextBox());
			this->AllSearchLabel = (gcnew System::Windows::Forms::Label());
			this->TransmitGroup = (gcnew System::Windows::Forms::GroupBox());
			this->FileDirectryReadButton = (gcnew System::Windows::Forms::Button());
			this->SystemIDLabel = (gcnew System::Windows::Forms::Label());
			this->RadioR = (gcnew System::Windows::Forms::RadioButton());
			this->RadioL = (gcnew System::Windows::Forms::RadioButton());
			this->ServerDirectryBox = (gcnew System::Windows::Forms::TextBox());
			this->SendCheck = (gcnew System::Windows::Forms::CheckBox());
			this->DataOutputGroup = (gcnew System::Windows::Forms::GroupBox());
			this->FrameNumber = (gcnew System::Windows::Forms::NumericUpDown());
			this->FrameNumLabel = (gcnew System::Windows::Forms::Label());
			this->SaveStartButton = (gcnew System::Windows::Forms::Button());
			this->FileNameBox = (gcnew System::Windows::Forms::TextBox());
			this->FileNameLabel = (gcnew System::Windows::Forms::Label());
			this->PanTilterGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->PanTilterEnableCheck = (gcnew System::Windows::Forms::CheckBox());
			this->PanTilterManualButton = (gcnew System::Windows::Forms::Button());
			this->AnglesBox = (gcnew System::Windows::Forms::GroupBox());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->PanAngle = (gcnew System::Windows::Forms::Label());
			this->PanAngleLabel = (gcnew System::Windows::Forms::Label());
			this->TiltAngleLabel = (gcnew System::Windows::Forms::Label());
			this->TiltAngle = (gcnew System::Windows::Forms::Label());
			this->UIView = (gcnew System::Windows::Forms::Timer(this->components));
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->leftImageBox))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rightImageBox))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->centerImageBox))->BeginInit();
			this->DistanceGroupBox->SuspendLayout();
			this->DistancesTable->SuspendLayout();
			this->RevisionGroup->SuspendLayout();
			this->ThresholdsGroup->SuspendLayout();
			this->TransmitGroup->SuspendLayout();
			this->DataOutputGroup->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->FrameNumber))->BeginInit();
			this->PanTilterGroupBox->SuspendLayout();
			this->AnglesBox->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// mainCount
			// 
			this->mainCount->Enabled = true;
			this->mainCount->Interval = 1;
			this->mainCount->Tick += gcnew System::EventHandler(this, &MainWindow::mainCount_Tick);
			// 
			// leftImageBox
			// 
			this->leftImageBox->Location = System::Drawing::Point(12, 258);
			this->leftImageBox->Name = L"leftImageBox";
			this->leftImageBox->Size = System::Drawing::Size(320, 240);
			this->leftImageBox->TabIndex = 0;
			this->leftImageBox->TabStop = false;
			// 
			// VCC_background
			// 
			this->VCC_background->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainWindow::VCC_background_DoWork);
			this->VCC_background->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &MainWindow::VCC_background_RunWorkerCompleted);
			// 
			// rightImageBox
			// 
			this->rightImageBox->Location = System::Drawing::Point(340, 258);
			this->rightImageBox->Name = L"rightImageBox";
			this->rightImageBox->Size = System::Drawing::Size(320, 240);
			this->rightImageBox->TabIndex = 1;
			this->rightImageBox->TabStop = false;
			// 
			// centerImageBox
			// 
			this->centerImageBox->Location = System::Drawing::Point(175, 12);
			this->centerImageBox->Name = L"centerImageBox";
			this->centerImageBox->Size = System::Drawing::Size(320, 240);
			this->centerImageBox->TabIndex = 2;
			this->centerImageBox->TabStop = false;
			// 
			// OriginalLabel
			// 
			this->OriginalLabel->AutoSize = true;
			this->OriginalLabel->Location = System::Drawing::Point(3, 0);
			this->OriginalLabel->Name = L"OriginalLabel";
			this->OriginalLabel->Size = System::Drawing::Size(44, 12);
			this->OriginalLabel->TabIndex = 3;
			this->OriginalLabel->Text = L"Original";
			// 
			// DistanceGroupBox
			// 
			this->DistanceGroupBox->Controls->Add(this->DistancesTable);
			this->DistanceGroupBox->Location = System::Drawing::Point(12, 12);
			this->DistanceGroupBox->Name = L"DistanceGroupBox";
			this->DistanceGroupBox->Size = System::Drawing::Size(157, 64);
			this->DistanceGroupBox->TabIndex = 4;
			this->DistanceGroupBox->TabStop = false;
			this->DistanceGroupBox->Text = L"Distances";
			// 
			// DistancesTable
			// 
			this->DistancesTable->ColumnCount = 2;
			this->DistancesTable->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				44.35484F)));
			this->DistancesTable->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				55.64516F)));
			this->DistancesTable->Controls->Add(this->KalmanValue, 1, 2);
			this->DistancesTable->Controls->Add(this->OriginalValue, 1, 0);
			this->DistancesTable->Controls->Add(this->OriginalLabel, 0, 0);
			this->DistancesTable->Controls->Add(this->RevisedLabel, 0, 1);
			this->DistancesTable->Controls->Add(this->KalmanLabel, 0, 2);
			this->DistancesTable->Controls->Add(this->RevisedValue, 1, 1);
			this->DistancesTable->Location = System::Drawing::Point(8, 15);
			this->DistancesTable->Name = L"DistancesTable";
			this->DistancesTable->RowCount = 3;
			this->DistancesTable->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->DistancesTable->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->DistancesTable->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->DistancesTable->Size = System::Drawing::Size(143, 43);
			this->DistancesTable->TabIndex = 4;
			// 
			// KalmanValue
			// 
			this->KalmanValue->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->KalmanValue->AutoSize = true;
			this->KalmanValue->Location = System::Drawing::Point(115, 28);
			this->KalmanValue->Name = L"KalmanValue";
			this->KalmanValue->Size = System::Drawing::Size(25, 12);
			this->KalmanValue->TabIndex = 9;
			this->KalmanValue->Text = L"0.00";
			this->KalmanValue->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// OriginalValue
			// 
			this->OriginalValue->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->OriginalValue->AutoSize = true;
			this->OriginalValue->Location = System::Drawing::Point(115, 0);
			this->OriginalValue->Name = L"OriginalValue";
			this->OriginalValue->Size = System::Drawing::Size(25, 12);
			this->OriginalValue->TabIndex = 5;
			this->OriginalValue->Text = L"0.00";
			this->OriginalValue->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// RevisedLabel
			// 
			this->RevisedLabel->AutoSize = true;
			this->RevisedLabel->Location = System::Drawing::Point(3, 14);
			this->RevisedLabel->Name = L"RevisedLabel";
			this->RevisedLabel->Size = System::Drawing::Size(46, 12);
			this->RevisedLabel->TabIndex = 4;
			this->RevisedLabel->Text = L"Revised";
			// 
			// KalmanLabel
			// 
			this->KalmanLabel->AutoSize = true;
			this->KalmanLabel->Location = System::Drawing::Point(3, 28);
			this->KalmanLabel->Name = L"KalmanLabel";
			this->KalmanLabel->Size = System::Drawing::Size(42, 12);
			this->KalmanLabel->TabIndex = 5;
			this->KalmanLabel->Text = L"Kalman";
			// 
			// RevisedValue
			// 
			this->RevisedValue->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->RevisedValue->AutoSize = true;
			this->RevisedValue->Location = System::Drawing::Point(115, 14);
			this->RevisedValue->Name = L"RevisedValue";
			this->RevisedValue->Size = System::Drawing::Size(25, 12);
			this->RevisedValue->TabIndex = 6;
			this->RevisedValue->Text = L"0.00";
			this->RevisedValue->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// Tracking_background
			// 
			this->Tracking_background->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainWindow::Tracking_background_DoWork);
			// 
			// RevisionGroup
			// 
			this->RevisionGroup->Controls->Add(this->RevisionToolButton);
			this->RevisionGroup->Location = System::Drawing::Point(13, 138);
			this->RevisionGroup->Name = L"RevisionGroup";
			this->RevisionGroup->Size = System::Drawing::Size(156, 39);
			this->RevisionGroup->TabIndex = 5;
			this->RevisionGroup->TabStop = false;
			this->RevisionGroup->Text = L"Revision";
			// 
			// RevisionToolButton
			// 
			this->RevisionToolButton->Location = System::Drawing::Point(83, 13);
			this->RevisionToolButton->Name = L"RevisionToolButton";
			this->RevisionToolButton->Size = System::Drawing::Size(67, 20);
			this->RevisionToolButton->TabIndex = 0;
			this->RevisionToolButton->Text = L"Tool";
			this->RevisionToolButton->UseVisualStyleBackColor = true;
			this->RevisionToolButton->Click += gcnew System::EventHandler(this, &MainWindow::RevisionToolButton_Click);
			// 
			// ThresholdsGroup
			// 
			this->ThresholdsGroup->Controls->Add(this->ReliabilityBox);
			this->ThresholdsGroup->Controls->Add(this->ReliabilityLabel);
			this->ThresholdsGroup->Controls->Add(this->TrackingThresholdBox);
			this->ThresholdsGroup->Controls->Add(this->TrackingThresholdLabel);
			this->ThresholdsGroup->Controls->Add(this->AllSearchBox);
			this->ThresholdsGroup->Controls->Add(this->AllSearchLabel);
			this->ThresholdsGroup->Location = System::Drawing::Point(503, 59);
			this->ThresholdsGroup->Name = L"ThresholdsGroup";
			this->ThresholdsGroup->Size = System::Drawing::Size(157, 99);
			this->ThresholdsGroup->TabIndex = 6;
			this->ThresholdsGroup->TabStop = false;
			this->ThresholdsGroup->Text = L"Thresholds";
			// 
			// ReliabilityBox
			// 
			this->ReliabilityBox->Location = System::Drawing::Point(97, 69);
			this->ReliabilityBox->Name = L"ReliabilityBox";
			this->ReliabilityBox->Size = System::Drawing::Size(49, 19);
			this->ReliabilityBox->TabIndex = 5;
			this->ReliabilityBox->Text = L"700";
			this->ReliabilityBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// ReliabilityLabel
			// 
			this->ReliabilityLabel->AutoSize = true;
			this->ReliabilityLabel->Location = System::Drawing::Point(7, 72);
			this->ReliabilityLabel->Name = L"ReliabilityLabel";
			this->ReliabilityLabel->Size = System::Drawing::Size(56, 12);
			this->ReliabilityLabel->TabIndex = 4;
			this->ReliabilityLabel->Text = L"Reliability";
			// 
			// TrackingThresholdBox
			// 
			this->TrackingThresholdBox->Location = System::Drawing::Point(97, 44);
			this->TrackingThresholdBox->Name = L"TrackingThresholdBox";
			this->TrackingThresholdBox->Size = System::Drawing::Size(49, 19);
			this->TrackingThresholdBox->TabIndex = 3;
			this->TrackingThresholdBox->Text = L"800";
			this->TrackingThresholdBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->TrackingThresholdBox->TextChanged += gcnew System::EventHandler(this, &MainWindow::TrackingThresholdBox_TextChanged);
			// 
			// TrackingThresholdLabel
			// 
			this->TrackingThresholdLabel->AutoSize = true;
			this->TrackingThresholdLabel->Location = System::Drawing::Point(7, 47);
			this->TrackingThresholdLabel->Name = L"TrackingThresholdLabel";
			this->TrackingThresholdLabel->Size = System::Drawing::Size(49, 12);
			this->TrackingThresholdLabel->TabIndex = 2;
			this->TrackingThresholdLabel->Text = L"Tracking";
			// 
			// AllSearchBox
			// 
			this->AllSearchBox->Location = System::Drawing::Point(97, 19);
			this->AllSearchBox->Name = L"AllSearchBox";
			this->AllSearchBox->Size = System::Drawing::Size(49, 19);
			this->AllSearchBox->TabIndex = 1;
			this->AllSearchBox->Text = L"800";
			this->AllSearchBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// AllSearchLabel
			// 
			this->AllSearchLabel->AutoSize = true;
			this->AllSearchLabel->Location = System::Drawing::Point(7, 22);
			this->AllSearchLabel->Name = L"AllSearchLabel";
			this->AllSearchLabel->Size = System::Drawing::Size(57, 12);
			this->AllSearchLabel->TabIndex = 0;
			this->AllSearchLabel->Text = L"All search";
			// 
			// TransmitGroup
			// 
			this->TransmitGroup->Controls->Add(this->FileDirectryReadButton);
			this->TransmitGroup->Controls->Add(this->SystemIDLabel);
			this->TransmitGroup->Controls->Add(this->RadioR);
			this->TransmitGroup->Controls->Add(this->RadioL);
			this->TransmitGroup->Controls->Add(this->ServerDirectryBox);
			this->TransmitGroup->Controls->Add(this->SendCheck);
			this->TransmitGroup->Location = System::Drawing::Point(503, 164);
			this->TransmitGroup->Name = L"TransmitGroup";
			this->TransmitGroup->Size = System::Drawing::Size(157, 88);
			this->TransmitGroup->TabIndex = 7;
			this->TransmitGroup->TabStop = false;
			this->TransmitGroup->Text = L"Transmit";
			// 
			// FileDirectryReadButton
			// 
			this->FileDirectryReadButton->Location = System::Drawing::Point(131, 59);
			this->FileDirectryReadButton->Name = L"FileDirectryReadButton";
			this->FileDirectryReadButton->Size = System::Drawing::Size(20, 19);
			this->FileDirectryReadButton->TabIndex = 5;
			this->FileDirectryReadButton->Text = L"...";
			this->FileDirectryReadButton->UseVisualStyleBackColor = true;
			// 
			// SystemIDLabel
			// 
			this->SystemIDLabel->AutoSize = true;
			this->SystemIDLabel->Location = System::Drawing::Point(23, 39);
			this->SystemIDLabel->Name = L"SystemIDLabel";
			this->SystemIDLabel->Size = System::Drawing::Size(58, 12);
			this->SystemIDLabel->TabIndex = 4;
			this->SystemIDLabel->Text = L"System ID";
			// 
			// RadioR
			// 
			this->RadioR->AutoSize = true;
			this->RadioR->Location = System::Drawing::Point(122, 37);
			this->RadioR->Name = L"RadioR";
			this->RadioR->Size = System::Drawing::Size(31, 16);
			this->RadioR->TabIndex = 3;
			this->RadioR->Text = L"R";
			this->RadioR->UseVisualStyleBackColor = true;
			// 
			// RadioL
			// 
			this->RadioL->AutoSize = true;
			this->RadioL->Checked = true;
			this->RadioL->Location = System::Drawing::Point(87, 37);
			this->RadioL->Name = L"RadioL";
			this->RadioL->Size = System::Drawing::Size(29, 16);
			this->RadioL->TabIndex = 2;
			this->RadioL->TabStop = true;
			this->RadioL->Text = L"L";
			this->RadioL->UseVisualStyleBackColor = true;
			// 
			// ServerDirectryBox
			// 
			this->ServerDirectryBox->Location = System::Drawing::Point(9, 59);
			this->ServerDirectryBox->Name = L"ServerDirectryBox";
			this->ServerDirectryBox->Size = System::Drawing::Size(116, 19);
			this->ServerDirectryBox->TabIndex = 1;
			this->ServerDirectryBox->Text = L"c:\\share";
			// 
			// SendCheck
			// 
			this->SendCheck->AutoSize = true;
			this->SendCheck->Location = System::Drawing::Point(9, 18);
			this->SendCheck->Name = L"SendCheck";
			this->SendCheck->Size = System::Drawing::Size(99, 16);
			this->SendCheck->TabIndex = 0;
			this->SendCheck->Text = L"Send to server";
			this->SendCheck->UseVisualStyleBackColor = true;
			// 
			// DataOutputGroup
			// 
			this->DataOutputGroup->Controls->Add(this->FrameNumber);
			this->DataOutputGroup->Controls->Add(this->FrameNumLabel);
			this->DataOutputGroup->Controls->Add(this->SaveStartButton);
			this->DataOutputGroup->Controls->Add(this->FileNameBox);
			this->DataOutputGroup->Controls->Add(this->FileNameLabel);
			this->DataOutputGroup->Location = System::Drawing::Point(13, 183);
			this->DataOutputGroup->Name = L"DataOutputGroup";
			this->DataOutputGroup->Size = System::Drawing::Size(156, 69);
			this->DataOutputGroup->TabIndex = 8;
			this->DataOutputGroup->TabStop = false;
			this->DataOutputGroup->Text = L"Data Output";
			// 
			// FrameNumber
			// 
			this->FrameNumber->Location = System::Drawing::Point(55, 43);
			this->FrameNumber->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000000, 0, 0, 0 });
			this->FrameNumber->Name = L"FrameNumber";
			this->FrameNumber->Size = System::Drawing::Size(43, 19);
			this->FrameNumber->TabIndex = 10;
			this->FrameNumber->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->FrameNumber->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100, 0, 0, 0 });
			// 
			// FrameNumLabel
			// 
			this->FrameNumLabel->AutoSize = true;
			this->FrameNumLabel->Location = System::Drawing::Point(11, 46);
			this->FrameNumLabel->Name = L"FrameNumLabel";
			this->FrameNumLabel->Size = System::Drawing::Size(37, 12);
			this->FrameNumLabel->TabIndex = 9;
			this->FrameNumLabel->Text = L"Frame";
			// 
			// SaveStartButton
			// 
			this->SaveStartButton->Location = System::Drawing::Point(104, 43);
			this->SaveStartButton->Name = L"SaveStartButton";
			this->SaveStartButton->Size = System::Drawing::Size(46, 19);
			this->SaveStartButton->TabIndex = 8;
			this->SaveStartButton->Text = L"Save";
			this->SaveStartButton->UseVisualStyleBackColor = true;
			this->SaveStartButton->Click += gcnew System::EventHandler(this, &MainWindow::SaveStartButton_Click);
			// 
			// FileNameBox
			// 
			this->FileNameBox->Location = System::Drawing::Point(72, 18);
			this->FileNameBox->Name = L"FileNameBox";
			this->FileNameBox->Size = System::Drawing::Size(78, 19);
			this->FileNameBox->TabIndex = 7;
			this->FileNameBox->Text = L"0";
			this->FileNameBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// FileNameLabel
			// 
			this->FileNameLabel->AutoSize = true;
			this->FileNameLabel->Location = System::Drawing::Point(11, 21);
			this->FileNameLabel->Name = L"FileNameLabel";
			this->FileNameLabel->Size = System::Drawing::Size(55, 12);
			this->FileNameLabel->TabIndex = 6;
			this->FileNameLabel->Text = L"File name";
			// 
			// PanTilterGroupBox
			// 
			this->PanTilterGroupBox->Controls->Add(this->PanTilterEnableCheck);
			this->PanTilterGroupBox->Controls->Add(this->PanTilterManualButton);
			this->PanTilterGroupBox->Location = System::Drawing::Point(503, 11);
			this->PanTilterGroupBox->Name = L"PanTilterGroupBox";
			this->PanTilterGroupBox->Size = System::Drawing::Size(157, 42);
			this->PanTilterGroupBox->TabIndex = 9;
			this->PanTilterGroupBox->TabStop = false;
			this->PanTilterGroupBox->Text = L"Pan-Tilter";
			// 
			// PanTilterEnableCheck
			// 
			this->PanTilterEnableCheck->AutoSize = true;
			this->PanTilterEnableCheck->Location = System::Drawing::Point(9, 18);
			this->PanTilterEnableCheck->Name = L"PanTilterEnableCheck";
			this->PanTilterEnableCheck->Size = System::Drawing::Size(58, 16);
			this->PanTilterEnableCheck->TabIndex = 1;
			this->PanTilterEnableCheck->Text = L"Enable";
			this->PanTilterEnableCheck->UseVisualStyleBackColor = true;
			this->PanTilterEnableCheck->CheckedChanged += gcnew System::EventHandler(this, &MainWindow::PanTilterEnableCheck_CheckedChanged);
			// 
			// PanTilterManualButton
			// 
			this->PanTilterManualButton->Enabled = false;
			this->PanTilterManualButton->Location = System::Drawing::Point(87, 15);
			this->PanTilterManualButton->Name = L"PanTilterManualButton";
			this->PanTilterManualButton->Size = System::Drawing::Size(59, 20);
			this->PanTilterManualButton->TabIndex = 0;
			this->PanTilterManualButton->Text = L"Manual";
			this->PanTilterManualButton->UseVisualStyleBackColor = true;
			this->PanTilterManualButton->Click += gcnew System::EventHandler(this, &MainWindow::PanTilterManualButton_Click);
			// 
			// AnglesBox
			// 
			this->AnglesBox->Controls->Add(this->tableLayoutPanel1);
			this->AnglesBox->Location = System::Drawing::Point(13, 82);
			this->AnglesBox->Name = L"AnglesBox";
			this->AnglesBox->Size = System::Drawing::Size(156, 50);
			this->AnglesBox->TabIndex = 5;
			this->AnglesBox->TabStop = false;
			this->AnglesBox->Text = L"Angles";
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				28.67133F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				71.32867F)));
			this->tableLayoutPanel1->Controls->Add(this->PanAngle, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->PanAngleLabel, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->TiltAngleLabel, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->TiltAngle, 1, 1);
			this->tableLayoutPanel1->Location = System::Drawing::Point(8, 15);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 2;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(143, 29);
			this->tableLayoutPanel1->TabIndex = 4;
			// 
			// PanAngle
			// 
			this->PanAngle->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->PanAngle->AutoSize = true;
			this->PanAngle->Location = System::Drawing::Point(103, 0);
			this->PanAngle->Name = L"PanAngle";
			this->PanAngle->Size = System::Drawing::Size(37, 12);
			this->PanAngle->TabIndex = 5;
			this->PanAngle->Text = L"0.0000";
			this->PanAngle->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// PanAngleLabel
			// 
			this->PanAngleLabel->AutoSize = true;
			this->PanAngleLabel->Location = System::Drawing::Point(3, 0);
			this->PanAngleLabel->Name = L"PanAngleLabel";
			this->PanAngleLabel->Size = System::Drawing::Size(24, 12);
			this->PanAngleLabel->TabIndex = 3;
			this->PanAngleLabel->Text = L"Pan";
			// 
			// TiltAngleLabel
			// 
			this->TiltAngleLabel->AutoSize = true;
			this->TiltAngleLabel->Location = System::Drawing::Point(3, 14);
			this->TiltAngleLabel->Name = L"TiltAngleLabel";
			this->TiltAngleLabel->Size = System::Drawing::Size(22, 12);
			this->TiltAngleLabel->TabIndex = 4;
			this->TiltAngleLabel->Text = L"Tilt";
			// 
			// TiltAngle
			// 
			this->TiltAngle->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->TiltAngle->AutoSize = true;
			this->TiltAngle->Location = System::Drawing::Point(103, 14);
			this->TiltAngle->Name = L"TiltAngle";
			this->TiltAngle->Size = System::Drawing::Size(37, 12);
			this->TiltAngle->TabIndex = 6;
			this->TiltAngle->Text = L"0.0000";
			this->TiltAngle->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// UIView
			// 
			this->UIView->Enabled = true;
			this->UIView->Interval = 30;
			this->UIView->Tick += gcnew System::EventHandler(this, &MainWindow::UIView_Tick);
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(672, 507);
			this->Controls->Add(this->AnglesBox);
			this->Controls->Add(this->PanTilterGroupBox);
			this->Controls->Add(this->DataOutputGroup);
			this->Controls->Add(this->TransmitGroup);
			this->Controls->Add(this->ThresholdsGroup);
			this->Controls->Add(this->RevisionGroup);
			this->Controls->Add(this->centerImageBox);
			this->Controls->Add(this->rightImageBox);
			this->Controls->Add(this->leftImageBox);
			this->Controls->Add(this->DistanceGroupBox);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MainWindow";
			this->Text = L"MainWindow";
			this->Load += gcnew System::EventHandler(this, &MainWindow::MainWindow_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->leftImageBox))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rightImageBox))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->centerImageBox))->EndInit();
			this->DistanceGroupBox->ResumeLayout(false);
			this->DistancesTable->ResumeLayout(false);
			this->DistancesTable->PerformLayout();
			this->RevisionGroup->ResumeLayout(false);
			this->ThresholdsGroup->ResumeLayout(false);
			this->ThresholdsGroup->PerformLayout();
			this->TransmitGroup->ResumeLayout(false);
			this->TransmitGroup->PerformLayout();
			this->DataOutputGroup->ResumeLayout(false);
			this->DataOutputGroup->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->FrameNumber))->EndInit();
			this->PanTilterGroupBox->ResumeLayout(false);
			this->PanTilterGroupBox->PerformLayout();
			this->AnglesBox->ResumeLayout(false);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void MainWindow_Load(System::Object^  sender, System::EventArgs^  e) {
	}
	///////////////////////////////////////////////以下自前追加関数////////////////////////////////////////
	/// std::string -> System::String への変換
	public: String^ ToSystemString(const std::string& i_src){
		return  gcnew System::String(i_src.data(), 0, i_src.size());
	}
	/// System::String^ -> std::string の変換
	public: std::string ToStdString(String^ i_src){
		using namespace Runtime::InteropServices;
		const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(i_src)).ToPointer();
		std::string os = chars;
		Marshal::FreeHGlobal(IntPtr((void*)chars));
		return os;
	}
	private: cv::Point *leftPoint, *rightPoint, *centerPoint;
	private: VCC ^vcc_L, ^vcc_R, ^vcc_C;
	private: ViCap *cameraL, *cameraR, *cameraC;
	public: PTU *ptu;
	public: bool vccFinish;
	private: System::Void viInit();/////////////////////////////////////////////////////////////
	private: System::Void mesuring();/////////////////////////////////////////////////////////////
	public: System::Void VCC_Matching(VCC ^vcc);
	private: System::Void mainCount_Tick(System::Object^  sender, System::EventArgs^  e) {
		//距離の計測
		mesuring();

		//記録関係
		if (output != nullptr){
			output->writeData(
				distances.original, 
				distances.revised, 
				distances.kf, 
				leftPoint->x + vcc_L->subpixel_x,
				leftPoint->y + vcc_L->subpixel_y,
				vcc_L->correlationPoint,
				rightPoint->x + vcc_R->subpixel_x,
				rightPoint->y + vcc_R->subpixel_y,
				vcc_R->correlationPoint
				);
			if (output->imageFrameIndex > Convert::ToInt32(this->FrameNumber->Text)){
				this->SaveStartButton->Enabled = true;
				output->~DataOutputFile();
				delete output;
				output = nullptr;
			}
		}
		//サーバー出力
		server_output();
	}
	private: System::Void VCC_background_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e);
	private: System::Void Tracking_background_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e);
	public: System::Void readInitFile(std::string filename);
	public: ref class initialParameters{
	public:
		ref class Camera{
		public:
			int id;
			double focalLength;
		}leftCamera,rightCamera,centerCamera;
		ref class Kalmanfilter{
		public:
			double processNoiseCov;
			double measurementNoiseCov;
		}measurementKalmanfilter,angleKalmanfilter;
		double baselineLength;
		double pixelSize;
		int COMPortNum;
		System::String ^serverDirectry;
		int clientID;
		bool automaticallyTransmit;
	}initParams;
	public: ref class Linear{
	public:
		double a0;
		double a1;
		double a2;
	}linear;
	public: ref class TrackingParameters{
	public:
		int centerCameraID;
		double centerCameraFocalLength;
		double centerCameraPixelSize;
		int COMPortNum;
	}trackingParameters;
private: System::Void RevisionToolButton_Click(System::Object^  sender, System::EventArgs^  e) {
	RevisionTool ^rt = gcnew RevisionTool();
	rt->Owner = this;
	rt->Show();
}
private: System::Void VCC_background_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e);
public: Threading::Mutex ^mtx;
public: Threading::Mutex ^tracking_mtx;
public: System::Void initialize();
public: ref class Distances{
public:
	double original;
	double revised;
	double kf;
}distances;
public: ref class Threshold{
public:
	int allSearch;
	int tracking;
	int reliability;
}leftThreshold,rightThreshold,centerThreshold;
public: System::Void readThreshold();
public: ref class KalmanFilterParameters{
public:
	KalmanFilterParameters(){
		prev_angles = gcnew array<double>(5);
		kalmanInitFinish = false;
	}
	System::Void kalmanInitialize(double processNoisCov, double measurementNoiseCov, double distance);
	cv::KalmanFilter *KF;
	cv::Mat_<float> *KF_State;
	cv::Mat *KF_ProcessNoise;
	cv::Mat_<float> *KF_Measurement;
	double prev_angle;
	bool kalmanInitFinish;
	double angleKalman(double platform, double ditected, int correlation, int tracking_th);
	array<double> ^prev_angles;
}distanceKalman,panAngleKalman,tiltAngleKalman;

public: ref class PanTiltAngle{
public: 
	double pan;
	double tilt;
}targetAngle;
private: int trackingThreshold;
private: System::Void TrackingThresholdBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	try{
		this->tracking_mtx->WaitOne();
		this->trackingThreshold = Convert::ToInt32(this->TrackingThresholdBox->Text);
	}
	finally{
		this->tracking_mtx->ReleaseMutex();
	}
}
private: System::Void UIView_Tick(System::Object^  sender, System::EventArgs^  e) {
	try{
		this->tracking_mtx->WaitOne();
		this->centerThreshold.allSearch = Convert::ToInt32(this->AllSearchBox->Text);
		this->centerThreshold.tracking = Convert::ToInt32(this->TrackingThresholdBox->Text);
		this->centerThreshold.reliability = Convert::ToInt32(this->ReliabilityBox->Text);
		this->PanAngle->Text = String::Format("{0:#0.0000}", (this->targetAngle.pan) * 180 / CV_PI);
		this->TiltAngle->Text = String::Format("{0:#0.0000}", (this->targetAngle.tilt) * 180 / CV_PI);
	}
	finally{
		this->tracking_mtx->ReleaseMutex();
	}
}
public: ref class DataOutputFile{
public:
	StreamWriter ^swriter;
	DataOutputFile(String ^filename){
		String ^filenamecsv;
		filenamecsv = "savedata\\" + filename + ".csv";
		swriter = gcnew StreamWriter(filenamecsv, false);
		imageFrameIndex = 1;
	}
	~DataOutputFile(){
		swriter->Close();
	
	}
	System::Void writeData(double original,
						   double revised,
						   double kalman,
						   double left_vcc_x,
						   double left_vcc_y,
						   double left_vcc_corr,
						   double right_vcc_x,
						   double right_vcc_y,
						   double right_vcc_corr
						   ){
		swriter->Write(String::Format("{0:#0.0000000000}", original));
		swriter->Write(",");
		swriter->Write(String::Format("{0:#0.0000000000}", revised));
		swriter->Write(",");
		swriter->Write(String::Format("{0:#0.0000000000}", kalman));
		swriter->Write(",");
		swriter->Write(String::Format("{0:#0.0000000000}", left_vcc_x));
		swriter->Write(",");
		swriter->Write(String::Format("{0:#0.0000000000}", left_vcc_y));
		swriter->Write(",");
		swriter->Write(String::Format("{0:#0.0000000000}", left_vcc_corr));
		swriter->Write(",");
		swriter->Write(String::Format("{0:#0.0000000000}", right_vcc_x));
		swriter->Write(",");
		swriter->Write(String::Format("{0:#0.0000000000}", right_vcc_y));
		swriter->Write(",");
		swriter->WriteLine(String::Format("{0:#0.0000000000}", right_vcc_corr));
		this->imageFrameIndex++;
	}
	int imageFrameIndex;
};
public: DataOutputFile ^output;
private: System::Void SaveStartButton_Click(System::Object^  sender, System::EventArgs^  e) {
	if (output != nullptr){
		output->~DataOutputFile();
		delete output;
		output = nullptr;
	}
	output = gcnew DataOutputFile(this->FileNameBox->Text);
	this->SaveStartButton->Enabled = false;
}
private: System::Void server_output(){
	if (this->SendCheck->Checked){
		String ^dir = this->ServerDirectryBox->Text;
		Console::Write("Sent to ");
		Console::WriteLine(dir);
		if (this->RadioL->Checked) dir += "\\L.dat";
		if (this->RadioR->Checked) dir += "\\R.dat";
		StreamWriter^ swriter;
		try{
			std::cout << "Try StreamWriter" << std::endl;
			FileStream ^fs = gcnew FileStream(dir, FileMode::Open, FileAccess::Write, FileShare::ReadWrite);
			swriter = gcnew StreamWriter(fs);
		}
		catch (FileNotFoundException ^ex)
		{
			std::cout << "FileNotFoundException" << std::endl;
			return;
		}
		catch (DirectoryNotFoundException ^ex)
		{
			std::cout << "FileNotFoundException" << std::endl;
			return;
		}
		catch (ArgumentException ^ex){
			std::cout << "ArgumentException" << std::endl;
			return;
		}
		swriter->WriteLine(this->distances.kf);
		try{
			this->tracking_mtx->WaitOne();
			swriter->WriteLine(this->targetAngle.pan);
			swriter->WriteLine(this->targetAngle.tilt);
		}
		finally{
			this->tracking_mtx->ReleaseMutex();
		}
		swriter->Close();
	}
}

private: System::Void PanTilterEnableCheck_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	this->PanTilterManualButton->Enabled = this->PanTilterEnableCheck->Checked;
}
		 public: bool ManualControlFlag;
private: System::Void PanTilterManualButton_Click(System::Object^  sender, System::EventArgs^  e) {
	Pantilter_manual ^ptm = gcnew Pantilter_manual();
	ptm->Owner = this;
	ptm->Show();
}
};
}
