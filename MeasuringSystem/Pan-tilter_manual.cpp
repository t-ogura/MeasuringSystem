#include "MainWindow.h"

using namespace MeasuringSystem;

System::Void Pantilter_manual::Manual_checkbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		bool state = Manual_checkbox->Checked;
		this->Up_button->Enabled = state;
		this->Left_button->Enabled = state;
		this->Right_button->Enabled = state;
		this->Down_button->Enabled = state;
		this->Home_button->Enabled = state;
		this->Auto_button->Enabled = state;
		this->DegNum->Enabled = state;

		MeasuringSystem::MainWindow ^mw;
		mw = static_cast<MainWindow^>(this->Owner);
		try{
			mw->tracking_mtx->WaitOne();
			mw->ManualControlFlag = state;
		}
		finally{
			mw->tracking_mtx->ReleaseMutex();
		}

	}
System::Void Pantilter_manual::Auto_button_Click(System::Object^  sender, System::EventArgs^  e) {
	MainWindow ^mw;
	mw = static_cast<MainWindow^>(this->Owner);
	try{
		mw->tracking_mtx->WaitOne();
		mw->ManualControlFlag = false;
	}
	finally{
		mw->tracking_mtx->ReleaseMutex();
	}
	Manual_checkbox->Checked = false;
}


System::Void Pantilter_manual::Up_button_Click(System::Object^  sender, System::EventArgs^  e){
	MainWindow ^mw;
	mw = static_cast<MainWindow^>(this->Owner);
	try{
		mw->tracking_mtx->WaitOne();
		mw->ptu->turn(0,Convert::ToDouble(this->DegNum->Value)*CV_PI/-180.);
	}
	finally{
		mw->tracking_mtx->ReleaseMutex();
	}
}


System::Void Pantilter_manual::Left_button_Click(System::Object^  sender, System::EventArgs^  e){
	MainWindow ^mw;
	mw = static_cast<MainWindow^>(this->Owner);
	try{
		mw->tracking_mtx->WaitOne();
		mw->ptu->turn(Convert::ToDouble(this->DegNum->Value)*CV_PI / -180., 0);
	}
	finally{
		mw->tracking_mtx->ReleaseMutex();
	}
}

System::Void Pantilter_manual::Right_button_Click(System::Object^  sender, System::EventArgs^  e){
	MainWindow ^mw;
	mw = static_cast<MainWindow^>(this->Owner);
	try{
		mw->tracking_mtx->WaitOne();
		mw->ptu->turn(Convert::ToDouble(this->DegNum->Value)*CV_PI / 180., 0);
	}
	finally{
		mw->tracking_mtx->ReleaseMutex();
	}
}

System::Void Pantilter_manual::Down_button_Click(System::Object^  sender, System::EventArgs^  e){
	MainWindow ^mw;
	mw = static_cast<MainWindow^>(this->Owner);
	try{
		mw->tracking_mtx->WaitOne();
		mw->ptu->turn(0, Convert::ToDouble(this->DegNum->Value)*CV_PI / 180.);
	}
	finally{
		mw->tracking_mtx->ReleaseMutex();
	}
}

System::Void Pantilter_manual::Home_button_Click(System::Object^  sender, System::EventArgs^  e){
	MainWindow ^mw;
	mw = static_cast<MainWindow^>(this->Owner);
	try{
		mw->tracking_mtx->WaitOne();
		mw->ptu->turnHome();;
	}
	finally{
		mw->tracking_mtx->ReleaseMutex();
	}
}