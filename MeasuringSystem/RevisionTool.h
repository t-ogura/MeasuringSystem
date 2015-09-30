#pragma once
#include <Windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "approx.h"
#include <vector>
#include <sstream>
#include <iostream>

namespace MeasuringSystem {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// RevisionTool の概要
	/// </summary>
	public ref class RevisionTool : public System::Windows::Forms::Form
	{
	public:
		RevisionTool(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクター コードを追加します
			//
		}

		/// std::string -> System::String への変換
		String^ ToSystemString(const std::string& i_src){
			return  gcnew System::String(i_src.data(), 0, i_src.size());
		}
		/// System::String^ -> std::string の変換
		std::string ToStdString(String^ i_src){
			using namespace Runtime::InteropServices;
			const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(i_src)).ToPointer();
			std::string os = chars;
			Marshal::FreeHGlobal(IntPtr((void*)chars));
			return os;
		}

		
	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~RevisionTool()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:

	private: System::Windows::Forms::TextBox^  linear_a2;


	private: System::Windows::Forms::TextBox^  true_value_box;


	private: System::Windows::Forms::Label^  true_value_label;
	private: System::Windows::Forms::TextBox^  measure_vaule_box;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  get_distance_button;
	private: System::Windows::Forms::Button^  add_button;
	private: System::Windows::Forms::ListBox^  true_list;
	private: System::Windows::Forms::ListBox^  measure_list;








	private: System::Windows::Forms::Button^  delete_button;
	private: System::Windows::Forms::Button^  exit_button;


	private: System::Windows::Forms::Button^  import;
	private: System::Windows::Forms::Button^  output;
	private: System::Windows::Forms::Button^  up_button;
	private: System::Windows::Forms::Button^  down_button;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::TextBox^  base_length;
	private: System::Windows::Forms::Label^  base_length_label;

	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
	private: System::Windows::Forms::TextBox^  linear_a1;


	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  linear_a0;



	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;









	private: System::ComponentModel::IContainer^  components;

	protected:

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
			this->linear_a2 = (gcnew System::Windows::Forms::TextBox());
			this->true_value_box = (gcnew System::Windows::Forms::TextBox());
			this->true_value_label = (gcnew System::Windows::Forms::Label());
			this->measure_vaule_box = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->get_distance_button = (gcnew System::Windows::Forms::Button());
			this->add_button = (gcnew System::Windows::Forms::Button());
			this->true_list = (gcnew System::Windows::Forms::ListBox());
			this->measure_list = (gcnew System::Windows::Forms::ListBox());
			this->delete_button = (gcnew System::Windows::Forms::Button());
			this->exit_button = (gcnew System::Windows::Forms::Button());
			this->import = (gcnew System::Windows::Forms::Button());
			this->output = (gcnew System::Windows::Forms::Button());
			this->up_button = (gcnew System::Windows::Forms::Button());
			this->down_button = (gcnew System::Windows::Forms::Button());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->base_length = (gcnew System::Windows::Forms::TextBox());
			this->base_length_label = (gcnew System::Windows::Forms::Label());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->linear_a1 = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->linear_a0 = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->statusStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// linear_a2
			// 
			this->linear_a2->Location = System::Drawing::Point(140, 26);
			this->linear_a2->Name = L"linear_a2";
			this->linear_a2->Size = System::Drawing::Size(113, 19);
			this->linear_a2->TabIndex = 2;
			this->linear_a2->Text = L"0.0";
			this->linear_a2->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// true_value_box
			// 
			this->true_value_box->Location = System::Drawing::Point(56, 112);
			this->true_value_box->Name = L"true_value_box";
			this->true_value_box->Size = System::Drawing::Size(53, 19);
			this->true_value_box->TabIndex = 5;
			this->true_value_box->Text = L"0.0";
			this->true_value_box->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->true_value_box->Click += gcnew System::EventHandler(this, &RevisionTool::true_value_box_Click);
			this->true_value_box->TextChanged += gcnew System::EventHandler(this, &RevisionTool::true_value_box_TextChanged);
			// 
			// true_value_label
			// 
			this->true_value_label->AutoSize = true;
			this->true_value_label->Location = System::Drawing::Point(13, 115);
			this->true_value_label->Name = L"true_value_label";
			this->true_value_label->Size = System::Drawing::Size(43, 12);
			this->true_value_label->TabIndex = 4;
			this->true_value_label->Text = L"Correct";
			// 
			// measure_vaule_box
			// 
			this->measure_vaule_box->Location = System::Drawing::Point(163, 112);
			this->measure_vaule_box->Name = L"measure_vaule_box";
			this->measure_vaule_box->Size = System::Drawing::Size(56, 19);
			this->measure_vaule_box->TabIndex = 7;
			this->measure_vaule_box->Text = L"0.0";
			this->measure_vaule_box->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->measure_vaule_box->Click += gcnew System::EventHandler(this, &RevisionTool::measure_vaule_box_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(115, 115);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(48, 12);
			this->label1->TabIndex = 6;
			this->label1->Text = L"Measure";
			this->label1->Click += gcnew System::EventHandler(this, &RevisionTool::label1_Click);
			// 
			// get_distance_button
			// 
			this->get_distance_button->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 8.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(128)));
			this->get_distance_button->Location = System::Drawing::Point(225, 112);
			this->get_distance_button->Name = L"get_distance_button";
			this->get_distance_button->Size = System::Drawing::Size(47, 19);
			this->get_distance_button->TabIndex = 8;
			this->get_distance_button->Text = L"Get";
			this->get_distance_button->UseVisualStyleBackColor = true;
			this->get_distance_button->Click += gcnew System::EventHandler(this, &RevisionTool::get_distance_button_Click);
			// 
			// add_button
			// 
			this->add_button->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 8.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(128)));
			this->add_button->Location = System::Drawing::Point(214, 137);
			this->add_button->Name = L"add_button";
			this->add_button->Size = System::Drawing::Size(58, 19);
			this->add_button->TabIndex = 9;
			this->add_button->Text = L"Add";
			this->add_button->UseVisualStyleBackColor = true;
			this->add_button->Click += gcnew System::EventHandler(this, &RevisionTool::add_button_Click);
			// 
			// true_list
			// 
			this->true_list->FormattingEnabled = true;
			this->true_list->ItemHeight = 12;
			this->true_list->Location = System::Drawing::Point(12, 137);
			this->true_list->Name = L"true_list";
			this->true_list->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
			this->true_list->Size = System::Drawing::Size(95, 184);
			this->true_list->TabIndex = 10;
			this->true_list->Click += gcnew System::EventHandler(this, &RevisionTool::true_list_Click);
			this->true_list->SelectedIndexChanged += gcnew System::EventHandler(this, &RevisionTool::true_list_SelectedIndexChanged);
			// 
			// measure_list
			// 
			this->measure_list->FormattingEnabled = true;
			this->measure_list->ItemHeight = 12;
			this->measure_list->Location = System::Drawing::Point(113, 137);
			this->measure_list->Name = L"measure_list";
			this->measure_list->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
			this->measure_list->Size = System::Drawing::Size(95, 184);
			this->measure_list->TabIndex = 11;
			this->measure_list->Click += gcnew System::EventHandler(this, &RevisionTool::measure_list_Click);
			this->measure_list->SelectedIndexChanged += gcnew System::EventHandler(this, &RevisionTool::measure_list_SelectedIndexChanged);
			// 
			// delete_button
			// 
			this->delete_button->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 8.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(128)));
			this->delete_button->Location = System::Drawing::Point(214, 212);
			this->delete_button->Name = L"delete_button";
			this->delete_button->Size = System::Drawing::Size(58, 19);
			this->delete_button->TabIndex = 15;
			this->delete_button->Text = L"Delete";
			this->delete_button->UseVisualStyleBackColor = true;
			this->delete_button->Click += gcnew System::EventHandler(this, &RevisionTool::delete_button_Click);
			// 
			// exit_button
			// 
			this->exit_button->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 8.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(128)));
			this->exit_button->Location = System::Drawing::Point(214, 300);
			this->exit_button->Name = L"exit_button";
			this->exit_button->Size = System::Drawing::Size(58, 19);
			this->exit_button->TabIndex = 16;
			this->exit_button->Text = L"Apply";
			this->exit_button->UseVisualStyleBackColor = true;
			this->exit_button->Click += gcnew System::EventHandler(this, &RevisionTool::exit_button_Click);
			// 
			// import
			// 
			this->import->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 8.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(128)));
			this->import->Location = System::Drawing::Point(214, 250);
			this->import->Name = L"import";
			this->import->Size = System::Drawing::Size(58, 19);
			this->import->TabIndex = 17;
			this->import->Text = L"Import";
			this->import->UseVisualStyleBackColor = true;
			this->import->Click += gcnew System::EventHandler(this, &RevisionTool::import_Click);
			// 
			// output
			// 
			this->output->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 8.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(128)));
			this->output->Location = System::Drawing::Point(214, 275);
			this->output->Name = L"output";
			this->output->Size = System::Drawing::Size(58, 19);
			this->output->TabIndex = 18;
			this->output->Text = L"Export";
			this->output->UseVisualStyleBackColor = true;
			this->output->Click += gcnew System::EventHandler(this, &RevisionTool::output_Click);
			// 
			// up_button
			// 
			this->up_button->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 8.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(128)));
			this->up_button->Location = System::Drawing::Point(214, 162);
			this->up_button->Name = L"up_button";
			this->up_button->Size = System::Drawing::Size(58, 19);
			this->up_button->TabIndex = 13;
			this->up_button->Text = L"Up";
			this->up_button->UseVisualStyleBackColor = true;
			this->up_button->Click += gcnew System::EventHandler(this, &RevisionTool::up_button_Click);
			// 
			// down_button
			// 
			this->down_button->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 8.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(128)));
			this->down_button->Location = System::Drawing::Point(214, 187);
			this->down_button->Name = L"down_button";
			this->down_button->Size = System::Drawing::Size(58, 19);
			this->down_button->TabIndex = 14;
			this->down_button->Text = L"Down";
			this->down_button->UseVisualStyleBackColor = true;
			this->down_button->Click += gcnew System::EventHandler(this, &RevisionTool::down_button_Click);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Tick += gcnew System::EventHandler(this, &RevisionTool::timer1_Tick);
			// 
			// base_length
			// 
			this->base_length->Location = System::Drawing::Point(15, 72);
			this->base_length->Name = L"base_length";
			this->base_length->Size = System::Drawing::Size(67, 19);
			this->base_length->TabIndex = 20;
			this->base_length->Text = L"300";
			this->base_length->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// base_length_label
			// 
			this->base_length_label->AutoSize = true;
			this->base_length_label->Location = System::Drawing::Point(13, 54);
			this->base_length_label->Name = L"base_length_label";
			this->base_length_label->Size = System::Drawing::Size(69, 12);
			this->base_length_label->TabIndex = 19;
			this->base_length_label->Text = L"Base Length";
			// 
			// saveFileDialog1
			// 
			this->saveFileDialog1->InitialDirectory = L"tool.cfg";
			this->saveFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &RevisionTool::saveFileDialog1_FileOk);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &RevisionTool::openFileDialog1_FileOk);
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->toolStripStatusLabel1 });
			this->statusStrip1->Location = System::Drawing::Point(0, 330);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(284, 23);
			this->statusStrip1->SizingGrip = false;
			this->statusStrip1->TabIndex = 22;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(271, 18);
			this->toolStripStatusLabel1->Text = L"Need 4+ data for computing correction value";
			this->toolStripStatusLabel1->Click += gcnew System::EventHandler(this, &RevisionTool::toolStripStatusLabel1_Click);
			// 
			// linear_a1
			// 
			this->linear_a1->Location = System::Drawing::Point(154, 51);
			this->linear_a1->Name = L"linear_a1";
			this->linear_a1->Size = System::Drawing::Size(99, 19);
			this->linear_a1->TabIndex = 24;
			this->linear_a1->Text = L"0.0";
			this->linear_a1->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(259, 29);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(11, 12);
			this->label2->TabIndex = 23;
			this->label2->Text = L"x";
			this->label2->Click += gcnew System::EventHandler(this, &RevisionTool::label2_Click);
			// 
			// linear_a0
			// 
			this->linear_a0->Location = System::Drawing::Point(154, 76);
			this->linear_a0->Name = L"linear_a0";
			this->linear_a0->Size = System::Drawing::Size(99, 19);
			this->linear_a0->TabIndex = 25;
			this->linear_a0->Text = L"0.0";
			this->linear_a0->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(259, 54);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(11, 12);
			this->label3->TabIndex = 26;
			this->label3->Text = L"x";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(128)));
			this->label4->Location = System::Drawing::Point(266, 25);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(11, 11);
			this->label4->TabIndex = 27;
			this->label4->Text = L"2";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(142, 79);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(11, 12);
			this->label5->TabIndex = 28;
			this->label5->Text = L"+";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(142, 54);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(11, 12);
			this->label6->TabIndex = 29;
			this->label6->Text = L"+";
			// 
			// label7
			// 
			this->label7->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label7->Location = System::Drawing::Point(12, 102);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(260, 2);
			this->label7->TabIndex = 30;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(138, 9);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(131, 12);
			this->label8->TabIndex = 31;
			this->label8->Text = L"Quadratic Approximation";
			// 
			// RevisionTool
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 353);
			this->ControlBox = false;
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->linear_a0);
			this->Controls->Add(this->linear_a1);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->base_length);
			this->Controls->Add(this->base_length_label);
			this->Controls->Add(this->output);
			this->Controls->Add(this->import);
			this->Controls->Add(this->exit_button);
			this->Controls->Add(this->delete_button);
			this->Controls->Add(this->down_button);
			this->Controls->Add(this->up_button);
			this->Controls->Add(this->measure_list);
			this->Controls->Add(this->true_list);
			this->Controls->Add(this->add_button);
			this->Controls->Add(this->get_distance_button);
			this->Controls->Add(this->measure_vaule_box);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->true_value_box);
			this->Controls->Add(this->true_value_label);
			this->Controls->Add(this->linear_a2);
			this->Controls->Add(this->label2);
			this->Name = L"RevisionTool";
			this->Text = L"RevisionTool";
			this->Load += gcnew System::EventHandler(this, &RevisionTool::RevisionTool_Load);
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void treeView1_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
	}
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
	}

	private: System::Void add_button_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (this->true_list->SelectedIndex >= 0) {
					 this->true_list->Items->Insert(this->true_list->SelectedIndex, this->true_value_box->Text);
					 this->true_list->SetSelected(this->true_list->SelectedIndex - 1, true);
				 }
				 else {
					 this->true_list->Items->Add(this->true_value_box->Text);
					 this->true_list->SetSelected(0, true);
				 }
				 if (this->measure_list->SelectedIndex >= 0) {
					 this->measure_list->Items->Insert(this->measure_list->SelectedIndex, this->measure_vaule_box->Text);
					 this->measure_list->SetSelected(this->measure_list->SelectedIndex - 1, true);
				 }
				 else {
					 this->measure_list->Items->Add(this->measure_vaule_box->Text);
					 this->measure_list->SetSelected(0, true);
				 }
	}
	private: System::Void true_list_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void measure_list_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void measure_list_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (this->measure_list->SelectedIndex < 0) return;
				 this->true_list->SetSelected(this->measure_list->SelectedIndex, true);
	}
	private: System::Void true_list_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (this->true_list->SelectedIndex < 0) return;
				 this->measure_list->SetSelected(this->true_list->SelectedIndex, true);
	}
	private: System::Void delete_button_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (this->true_list->SelectedIndex < 0 || this->measure_list->SelectedIndex < 0) return;
				 int t = this->true_list->SelectedIndex;
				 int m = this->measure_list->SelectedIndex;
				 this->true_list->Items->RemoveAt(t);
				 this->measure_list->Items->RemoveAt(m);
				 if (t == this->true_list->Items->Count || m == this->measure_list->Items->Count) return;
				 this->true_list->SetSelected(t, true);
				 this->measure_list->SetSelected(m, true);

	}
	private: System::Void exit_button_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->ApplyOffset();
				 this->savePrevData();
				 this->~RevisionTool();
	}
	private: System::Void up_button_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (this->true_list->SelectedIndex <= 0 || this->measure_list->SelectedIndex <= 0) return;
				 int t = this->true_list->SelectedIndex;
				 int m = this->measure_list->SelectedIndex;
				 this->true_list->Items->Insert(t - 1, this->true_list->SelectedItem);
				 this->measure_list->Items->Insert(m - 1, this->measure_list->SelectedItem);
				 this->true_list->Items->RemoveAt(t + 1);
				 this->measure_list->Items->RemoveAt(m + 1);
				 this->true_list->SetSelected(t - 1, true);
				 this->measure_list->SetSelected(m - 1, true);
	}
	private: System::Void down_button_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (this->true_list->SelectedIndex < 0 || this->measure_list->SelectedIndex < 0) return;
				 if (this->true_list->SelectedIndex >= this->true_list->Items->Count - 1 || this->measure_list->SelectedIndex >= this->measure_list->Items->Count - 1) return;
				 int t = this->true_list->SelectedIndex;
				 int m = this->measure_list->SelectedIndex;
				 this->true_list->SetSelected(t + 1, true);
				 this->measure_list->SetSelected(m + 1, true);
				 this->true_list->Items->Insert(t, this->true_list->SelectedItem);
				 this->measure_list->Items->Insert(m, this->measure_list->SelectedItem);
				 this->true_list->Items->RemoveAt(t + 2);
				 this->measure_list->Items->RemoveAt(m + 2);
				 this->true_list->SetSelected(t + 1, true);
				 this->measure_list->SetSelected(m + 1, true);
	}
	private: System::Void RevisionTool_Load(System::Object^  sender, System::EventArgs^  e) {
	}
	public: bool base_length_read_flag;
	public: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e);
	public: System::Void get_distance_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void true_value_box_TextChanged(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void true_value_box_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->true_value_box->SelectAll();
}
private: System::Void measure_vaule_box_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->measure_vaule_box->SelectAll();
}
		 private: String ^fname;
				  private: int time_count;
private: System::Void output_Click(System::Object^  sender, System::EventArgs^  e) {

			 SaveFileDialog^ sfdlg = gcnew SaveFileDialog();
			 String ^path = Path::GetFullPath("tool.cfg\\");
			 sfdlg->InitialDirectory = path;
			 sfdlg->Filter = "Configuration file(*cfg)|*.cfg";
			 if (sfdlg->ShowDialog() != Windows::Forms::DialogResult::OK) return;
			 fname = sfdlg->FileName;
			 StreamWriter^ swriter = gcnew StreamWriter(fname);
			 swriter->WriteLine(this->base_length->Text);
			 for (int i = 0; i < this->true_list->Items->Count && i < this->measure_list->Items->Count; i++){
				 swriter->WriteLine(this->true_list->Items[i]);
				 swriter->WriteLine(this->measure_list->Items[i]);
			 }
			 swriter->Close();
			 this->time_count = 0;
			 String ^notice;
			 String ^split = "\\";
			 array<Char>^ splitchar = split->ToCharArray();
			 array<String^>^ words;
			 words = fname->Split(splitchar);
			 notice = "Exported file[";
			 notice += words[words->Length - 1];
			 notice += "]";
			 this->toolStripStatusLabel1->Text = notice;
}
private: System::Void saveFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
}
private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
}
private: System::Void import_Click(System::Object^  sender, System::EventArgs^  e) {

			 OpenFileDialog^ ofdlg = gcnew OpenFileDialog();
			 String ^path = Path::GetFullPath("tool.cfg\\");
			 ofdlg->InitialDirectory = path;
			 ofdlg->Filter = "Configuration file(*cfg)|*.cfg";
			 if (ofdlg->ShowDialog() != Windows::Forms::DialogResult::OK) return;
			 this->true_list->Items->Clear();
			 this->measure_list->Items->Clear();
			 fname = ofdlg->FileName;
			 StreamReader^ sreader = gcnew StreamReader(fname);
			 if (sreader->Peek() == -1)return;
			 this->base_length->Text = sreader->ReadLine();
			 while (sreader->Peek() > -1){
				 this->true_list->Items->Add(sreader->ReadLine());
				 this->measure_list->Items->Add(sreader->ReadLine());
			 }
			 sreader->Close();
			 Invalidate();
			 this->time_count = 0;
			 String ^notice;
			 String ^split = "\\";
			 array<Char>^ splitchar = split->ToCharArray();
			 array<String^>^ words;
			 words = fname->Split(splitchar);
			 notice = "Imported file[";
			 notice += words[words->Length-1];
			 notice += "]";
			 this->toolStripStatusLabel1->Text = notice;
}
private: System::Void toolStripStatusLabel1_Click(System::Object^  sender, System::EventArgs^  e) {
}
		 public: System::Void getBaseLength();
public: System::Void savePrevData(){
			 fname = "tool.cfg\\Temp.cfg";
			 StreamWriter^ swriter = gcnew StreamWriter(fname);
			 swriter->WriteLine(this->base_length->Text);
			 for (int i = 0; i < this->true_list->Items->Count && i < this->measure_list->Items->Count; i++){
				 swriter->WriteLine(this->true_list->Items[i]);
				 swriter->WriteLine(this->measure_list->Items[i]);
			 }
			 swriter->Close();
			 this->time_count = 0;
			 String ^notice;
			 String ^split = "\\";
			 array<Char>^ splitchar = split->ToCharArray();
			 array<String^>^ words;
			 words = fname->Split(splitchar);
			 notice = "Exported file[";
			 notice += words[words->Length - 1];
			 notice += "]";
			 this->toolStripStatusLabel1->Text = notice;
}
public: System::Void readPrevData(){

			 this->true_list->Items->Clear();
			 this->measure_list->Items->Clear();
			 fname = "tool.cfg\\Temp.cfg";
			 StreamReader^ sreader = gcnew StreamReader(fname);
			 if (sreader->Peek() == -1)return;
			 this->base_length->Text = sreader->ReadLine();
			 while (sreader->Peek() > -1){
				 this->true_list->Items->Add(sreader->ReadLine());
				 this->measure_list->Items->Add(sreader->ReadLine());
			 }
			 sreader->Close();
			 Invalidate();
			 this->time_count = 0;
			 String ^notice;
			 String ^split = "\\";
			 array<Char>^ splitchar = split->ToCharArray();
			 array<String^>^ words;
			 words = fname->Split(splitchar);
			 notice = "Import file[";
			 notice += words[words->Length - 1];
			 notice += "]";
			 this->toolStripStatusLabel1->Text = notice;
}
public: System::Void ApplyOffset();
private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
}
};
}
