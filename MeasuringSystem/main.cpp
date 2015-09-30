#include "MainWindow.h"


using namespace MeasuringSystem;

int main(){
	MainWindow ^mw = gcnew MainWindow();
	mw->ShowDialog();
	return 0;
}