#include "dialog_integratedvideo.h"
#include "ui_dialog_integratedvideo.h"
#include <QVector>
using namespace std;


Dialog_IntegratedVideo::Dialog_IntegratedVideo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_IntegratedVideo)
{
	
    ui->setupUi(this);
	ui->pushButton_15->setEnabled(false);
	ui->pushButton_16->setEnabled(false);
	ui->lineEdit_12->setAlignment(Qt::AlignTop);//contrain the space of the input
	
}

string Dialog_IntegratedVideo::filename = "";

Dialog_IntegratedVideo::~Dialog_IntegratedVideo()
{
    delete ui;
}

void Dialog_IntegratedVideo::setAddSubtitleCommand(std::shared_ptr<Command> command)
{
	ptr_addsubtitleCommand = command;
}

void Dialog_IntegratedVideo::setAddStickerCommand(std::shared_ptr<Command> command)
{
	ptr_addstickerCommand = command;
}

void Dialog_IntegratedVideo::setMirrorCommand(std::shared_ptr<Command> command)
{
	ptr_addmirrorCommand = command;
}
    
void Dialog_IntegratedVideo::setRotateCommand(std::shared_ptr<Command> command)
{
	ptr_addrotateCommand = command;
}

void Dialog_IntegratedVideo::setAddFilterCommand(std::shared_ptr<Command> command)
{
	ptr_addfilterCommand = command;
}

void Dialog_IntegratedVideo::setFunnyCommand(std::shared_ptr<Command> command)
{
	ptr_addfunnyCommand = command;
}

void Dialog_IntegratedVideo::setPlayVideoCommand(std::shared_ptr<Command> command)
{
    ptr_playvideoCommand = command;
}

void Dialog_IntegratedVideo::setExportVideoCommand(std::shared_ptr<Command> command)
{
	ptr_exportvideoCommand = command;
}

void Dialog_IntegratedVideo::on_pushButton_17_clicked()//Subtitle confirm button
{
	
	ui->pushButton_17->setEnabled(false);
	//get parameters
	string text = ui->lineEdit_12->text().toStdString();
	int startframe = ui->lineEdit_3->text().toInt();
	int endframe = ui->lineEdit_6->text().toInt();
	double x = ui->lineEdit_10->text().toDouble();
	double y = ui->lineEdit_11->text().toDouble();
	double size = ui->comboBox_2->currentText().toDouble();
	size = size / 10.0;
	double r = ui->lineEdit_7->text().toDouble();
	double g = ui->lineEdit_8->text().toDouble();
	double b = ui->lineEdit_9->text().toDouble();
	bool isBold = ui->checkBox_8->isChecked();
	//display vector
	/*res.push_back(QString::fromStdString(text));
	res.push_back(QString::number(start_frame, 10, 0));
	res.push_back(QString::number(end_frame, 10, 0));
	res.push_back(QString::number(x, 10, 1));
	res.push_back(QString::number(y, 10, 1));

	globalplay.PlayList[0].addSubtitle(text, start_frame, end_frame, x, y, fontSize, B, G, R, isBold);
	//globalplay.play(0);
	QMessageBox::about(0, QObject::tr("success"), "Add subtitle success!");
	for (int i = 0; i < (int)(res.size() / 5); i++)
	{
		for (int j = 0; j < 5; j++)
		{
			ui->tableWidget_4->setItem(i, j, new QTableWidgetItem(res[5*i+j]));
		}
	}*/
	ptr_addsubtitleCommand->set_parameters(std::static_pointer_cast<Parameters, SubtitleParameters>(std::shared_ptr<SubtitleParameters>(new SubtitleParameters(text,startframe,endframe,x,y,r,g,b,size,isBold))));
    ptr_addsubtitleCommand->exec();

	ui->pushButton_17->setEnabled(true);
	ui->lineEdit_12->clear();
	ui->lineEdit_3->clear();
	ui->lineEdit_6->clear();
	ui->lineEdit_10->clear();
	ui->lineEdit_11->clear();
	ui->lineEdit_7->clear();
	ui->lineEdit_8->clear();
	ui->lineEdit_9->clear();

}

void Dialog_IntegratedVideo::on_preview_clicked()//preview button
{
	ptr_playvideoCommand->set_parameters(std::static_pointer_cast<Parameters, IntParameters>(std::shared_ptr<IntParameters>(new IntParameters(0))));
    ptr_playvideoCommand->exec();
}
/*
void Dialog_IntegratedVideo::on_pushButton_clicked()//display the size of video
{
	//display size
	double width = globalplay.PlayList[0].getWidth();
	double height = globalplay.PlayList[0].getHeight();
	ui->lineEdit_13->setText(QString::number(width, 10, 1));
	ui->lineEdit_14->setText(QString::number(height, 10, 1));
}*/

void Dialog_IntegratedVideo::on_pushButton_4_clicked()//add special effects
{
	ui->pushButton_4->setEnabled(false);

	if (ui->checkBox->isChecked())//mirror 
	{
		int startMirror = ui->lineEdit->text().toInt();
		int endMirror = ui->lineEdit_2->text().toInt();
		ptr_addmirrorCommand->set_parameters(std::static_pointer_cast<Parameters, FilterParameters>(std::shared_ptr<FilterParameters>(new FilterParameters(startMirror,endMirror,0))));
        ptr_addmirrorCommand->exec();
	}

	else
	{
		ptr_addmirrorCommand->set_parameters(std::static_pointer_cast<Parameters, FilterParameters>(std::shared_ptr<FilterParameters>(new FilterParameters(0,0,-1))));
        ptr_addmirrorCommand->exec();
	}

	if (ui->checkBox_2->isChecked())//rotation
	{
		int startRotation = ui->lineEdit_4->text().toInt();
		int endRotation = ui->lineEdit_5->text().toInt();
		ptr_addrotateCommand->set_parameters(std::static_pointer_cast<Parameters, FilterParameters>(std::shared_ptr<FilterParameters>(new FilterParameters(startRotation,endRotation,1))));
        ptr_addrotateCommand->exec();
	}

	else
	{
		ptr_addrotateCommand->set_parameters(std::static_pointer_cast<Parameters, FilterParameters>(std::shared_ptr<FilterParameters>(new FilterParameters(0,0,-1))));
        ptr_addrotateCommand->exec();
	}

	//filter
	if (ui->checkBox_3->isChecked() || ui->checkBox_4->isChecked() || ui->checkBox_5->isChecked() || ui->checkBox_6->isChecked() || ui->checkBox_7->isChecked())
	{
		int startFilter = ui->lineEdit_19->text().toInt();
		int endFilter = ui->lineEdit_20->text().toInt();

		if (ui->checkBox_3->isChecked())
		{
			ptr_addfilterCommand->set_parameters(std::static_pointer_cast<Parameters, FilterParameters>(std::shared_ptr<FilterParameters>(new FilterParameters(startFilter,endFilter,3))));
        	ptr_addfilterCommand->exec();
		}
		if (ui->checkBox_4->isChecked())
		{
			ptr_addfilterCommand->set_parameters(std::static_pointer_cast<Parameters, FilterParameters>(std::shared_ptr<FilterParameters>(new FilterParameters(startFilter,endFilter,4))));
        	ptr_addfilterCommand->exec();
		}
		if (ui->checkBox_5->isChecked())
		{
			ptr_addfilterCommand->set_parameters(std::static_pointer_cast<Parameters, FilterParameters>(std::shared_ptr<FilterParameters>(new FilterParameters(startFilter,endFilter,12))));
        	ptr_addfilterCommand->exec();
		}
		if (ui->checkBox_6->isChecked())
		{
			ptr_addfilterCommand->set_parameters(std::static_pointer_cast<Parameters, FilterParameters>(std::shared_ptr<FilterParameters>(new FilterParameters(startFilter,endFilter,14))));
        	ptr_addfilterCommand->exec();
		}
		if (ui->checkBox_7->isChecked())
		{
			ptr_addfilterCommand->set_parameters(std::static_pointer_cast<Parameters, FilterParameters>(std::shared_ptr<FilterParameters>(new FilterParameters(startFilter,endFilter,13))));
        	ptr_addfilterCommand->exec();
		}

	}

	else
	{
		ptr_addfilterCommand->set_parameters(std::static_pointer_cast<Parameters, FilterParameters>(std::shared_ptr<FilterParameters>(new FilterParameters(0,0,-1))));
        ptr_addfilterCommand->exec();
	}

	if (ui->checkBox_9->isChecked() || ui->checkBox_10->isChecked() || ui->checkBox_11->isChecked())//special effect
	{
		int start = ui->lineEdit_26->text().toInt();
		int end = ui->lineEdit_29->text().toInt();

		if (ui->checkBox_9->isChecked())
		{
			ptr_addfunnyCommand->set_parameters(std::static_pointer_cast<Parameters, FilterParameters>(std::shared_ptr<FilterParameters>(new FilterParameters(start,end,5))));
        	ptr_addfunnyCommand->exec();
		}
		if (ui->checkBox_10->isChecked())
		{
			ptr_addfunnyCommand->set_parameters(std::static_pointer_cast<Parameters, FilterParameters>(std::shared_ptr<FilterParameters>(new FilterParameters(start,end,6))));
        	ptr_addfunnyCommand->exec();
		}
		if (ui->checkBox_11->isChecked())
		{
			ptr_addfunnyCommand->set_parameters(std::static_pointer_cast<Parameters, FilterParameters>(std::shared_ptr<FilterParameters>(new FilterParameters(start,end,7))));
        	ptr_addfunnyCommand->exec();
		}
	}

	else
	{
		ptr_addfunnyCommand->set_parameters(std::static_pointer_cast<Parameters, FilterParameters>(std::shared_ptr<FilterParameters>(new FilterParameters(0,0,-1))));
        ptr_addfunnyCommand->exec();
	}
	//QMessageBox::about(0, QObject::tr("success"), "Add success!");
	ui->pushButton_4->setEnabled(true);
	ui->lineEdit->clear();
	ui->lineEdit_2->clear();
	ui->lineEdit_4->clear();
	ui->lineEdit_5->clear();
	ui->lineEdit_19->clear();
	ui->lineEdit_20->clear();
	ui->lineEdit_29->clear();
	ui->lineEdit_26->clear();
}

void Dialog_IntegratedVideo::on_pushButton_3_clicked()//stick pictures
{
	ui->pushButton_5->setEnabled(false);

	double x = ui->lineEdit_16->text().toDouble();
	double y = ui->lineEdit_15->text().toDouble();
	int start = ui->lineEdit_17->text().toInt();
	int end = ui->lineEdit_18->text().toInt();

	/*globalplay.PlayList[0].addSticker(filename, x, y, start, end);

	QMessageBox::about(0, QObject::tr("success"), "Stick success!");*/
	ptr_addstickerCommand->set_parameters(std::static_pointer_cast<Parameters, StickerParameters>(std::shared_ptr<StickerParameters>(new StickerParameters(Dialog_IntegratedVideo::filename,start,end,x,y))));
    ptr_addstickerCommand->exec();
	ui->lineEdit_15->clear();
	ui->lineEdit_16->clear();
	ui->lineEdit_17->clear();
	ui->lineEdit_18->clear();
	ui->pushButton_5->setEnabled(true);
}

void Dialog_IntegratedVideo::on_pushButton_2_clicked()//export
{	
	QString temp = QFileDialog::getSaveFileName(this,tr("Save File"),"",tr("*.avi;; *.mp4;; *.mov"));
	string filename = temp.toStdString();
	ptr_exportvideoCommand->set_parameters(std::static_pointer_cast<Parameters, PathParameters>(std::shared_ptr<PathParameters>(new PathParameters(filename))));
    ptr_exportvideoCommand->exec();
	QMessageBox::about(0, QObject::tr("success"), "Export success!");
}

void Dialog_IntegratedVideo::on_pushButton_5_clicked()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"));
	QString temp = fileNames.join("\\");
	Dialog_IntegratedVideo::filename = temp.toStdString();
}
