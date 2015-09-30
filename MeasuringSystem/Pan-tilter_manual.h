#pragma once

namespace MeasuringSystem {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Pantilter_manual の概要
	/// </summary>
	public ref class Pantilter_manual : public System::Windows::Forms::Form
	{
	public:
		Pantilter_manual(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクター コードを追加します
			//
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~Pantilter_manual()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::CheckBox^  Manual_checkbox;
	private: System::Windows::Forms::Button^  Up_button;
	private: System::Windows::Forms::Button^  Down_button;


	private: System::Windows::Forms::Button^  Left_button;

	private: System::Windows::Forms::Button^  Right_button;
	private: System::Windows::Forms::NumericUpDown^  DegNum;
	private: System::Windows::Forms::Label^  DegLabel;



	private: System::Windows::Forms::Button^  Home_button;
	private: System::Windows::Forms::Button^  Auto_button;


	protected:

	private:
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->Manual_checkbox = (gcnew System::Windows::Forms::CheckBox());
			this->Up_button = (gcnew System::Windows::Forms::Button());
			this->Down_button = (gcnew System::Windows::Forms::Button());
			this->Left_button = (gcnew System::Windows::Forms::Button());
			this->Right_button = (gcnew System::Windows::Forms::Button());
			this->DegNum = (gcnew System::Windows::Forms::NumericUpDown());
			this->DegLabel = (gcnew System::Windows::Forms::Label());
			this->Home_button = (gcnew System::Windows::Forms::Button());
			this->Auto_button = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->DegNum))->BeginInit();
			this->SuspendLayout();
			// 
			// Manual_checkbox
			// 
			this->Manual_checkbox->AutoSize = true;
			this->Manual_checkbox->Location = System::Drawing::Point(13, 13);
			this->Manual_checkbox->Name = L"Manual_checkbox";
			this->Manual_checkbox->Size = System::Drawing::Size(99, 16);
			this->Manual_checkbox->TabIndex = 0;
			this->Manual_checkbox->Text = L"Manual control";
			this->Manual_checkbox->UseVisualStyleBackColor = true;
			this->Manual_checkbox->CheckedChanged += gcnew System::EventHandler(this, &Pantilter_manual::Manual_checkbox_CheckedChanged);
			// 
			// Up_button
			// 
			this->Up_button->Enabled = false;
			this->Up_button->Location = System::Drawing::Point(66, 35);
			this->Up_button->Name = L"Up_button";
			this->Up_button->Size = System::Drawing::Size(35, 35);
			this->Up_button->TabIndex = 1;
			this->Up_button->Text = L"↑";
			this->Up_button->UseVisualStyleBackColor = true;
			this->Up_button->Click += gcnew System::EventHandler(this, &Pantilter_manual::Up_button_Click);
			// 
			// Down_button
			// 
			this->Down_button->Enabled = false;
			this->Down_button->Location = System::Drawing::Point(66, 125);
			this->Down_button->Name = L"Down_button";
			this->Down_button->Size = System::Drawing::Size(35, 35);
			this->Down_button->TabIndex = 2;
			this->Down_button->Text = L"↓";
			this->Down_button->UseVisualStyleBackColor = true;
			this->Down_button->Click += gcnew System::EventHandler(this, &Pantilter_manual::Down_button_Click);
			// 
			// Left_button
			// 
			this->Left_button->Enabled = false;
			this->Left_button->Location = System::Drawing::Point(13, 80);
			this->Left_button->Name = L"Left_button";
			this->Left_button->Size = System::Drawing::Size(35, 35);
			this->Left_button->TabIndex = 3;
			this->Left_button->Text = L"←";
			this->Left_button->UseVisualStyleBackColor = true;
			this->Left_button->Click += gcnew System::EventHandler(this, &Pantilter_manual::Left_button_Click);
			// 
			// Right_button
			// 
			this->Right_button->Enabled = false;
			this->Right_button->Location = System::Drawing::Point(119, 80);
			this->Right_button->Name = L"Right_button";
			this->Right_button->Size = System::Drawing::Size(35, 35);
			this->Right_button->TabIndex = 4;
			this->Right_button->Text = L"→";
			this->Right_button->UseVisualStyleBackColor = true;
			this->Right_button->Click += gcnew System::EventHandler(this, &Pantilter_manual::Right_button_Click);
			// 
			// DegNum
			// 
			this->DegNum->Enabled = false;
			this->DegNum->Location = System::Drawing::Point(55, 89);
			this->DegNum->Name = L"DegNum";
			this->DegNum->Size = System::Drawing::Size(35, 19);
			this->DegNum->TabIndex = 5;
			this->DegNum->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->DegNum->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 0 });
			// 
			// DegLabel
			// 
			this->DegLabel->AutoSize = true;
			this->DegLabel->Location = System::Drawing::Point(92, 92);
			this->DegLabel->Name = L"DegLabel";
			this->DegLabel->Size = System::Drawing::Size(25, 12);
			this->DegLabel->TabIndex = 6;
			this->DegLabel->Text = L"deg.";
			// 
			// Home_button
			// 
			this->Home_button->Enabled = false;
			this->Home_button->Location = System::Drawing::Point(12, 136);
			this->Home_button->Name = L"Home_button";
			this->Home_button->Size = System::Drawing::Size(47, 23);
			this->Home_button->TabIndex = 7;
			this->Home_button->Text = L"Home";
			this->Home_button->UseVisualStyleBackColor = true;
			this->Home_button->Click += gcnew System::EventHandler(this, &Pantilter_manual::Home_button_Click);
			// 
			// Auto_button
			// 
			this->Auto_button->Enabled = false;
			this->Auto_button->Location = System::Drawing::Point(108, 136);
			this->Auto_button->Name = L"Auto_button";
			this->Auto_button->Size = System::Drawing::Size(47, 23);
			this->Auto_button->TabIndex = 8;
			this->Auto_button->Text = L"Auto";
			this->Auto_button->UseVisualStyleBackColor = true;
			this->Auto_button->Click += gcnew System::EventHandler(this, &Pantilter_manual::Auto_button_Click);
			// 
			// Pantilter_manual
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(167, 172);
			this->Controls->Add(this->Auto_button);
			this->Controls->Add(this->Home_button);
			this->Controls->Add(this->DegNum);
			this->Controls->Add(this->DegLabel);
			this->Controls->Add(this->Right_button);
			this->Controls->Add(this->Left_button);
			this->Controls->Add(this->Down_button);
			this->Controls->Add(this->Up_button);
			this->Controls->Add(this->Manual_checkbox);
			this->MaximizeBox = false;
			this->Name = L"Pantilter_manual";
			this->ShowIcon = false;
			this->Text = L"Controller";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->DegNum))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Manual_checkbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void Auto_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void Up_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void Left_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void Right_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void Down_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void Home_button_Click(System::Object^  sender, System::EventArgs^  e);
};
}
