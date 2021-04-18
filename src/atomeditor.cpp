/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#include "atomeditor.h"
#include "ui_atomeditor.h"

#include <QDebug>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

void writePorts(QTreeWidgetItemIterator it, QXmlStreamWriter &out);
void readPorts(PortEditor* port, QXmlStreamReader &in);

AtomEditor::AtomEditor(QWidget *parent):
    BlockEditor(parent),
    ui(new Ui::AtomEditor)
{
    ui->setupUi(this);

    connect(ui->inputEditor, &PortEditor::edited,
            this, &AtomEditor::editedWork);
    connect(ui->outputEditor, &PortEditor::edited,
            this, &AtomEditor::editedWork);
    connect(ui->codeEditor, &QTextEdit::textChanged,
            this, &AtomEditor::editedWork);
    connect(ui->nameEditor, &QLineEdit::textChanged,
            this, &AtomEditor::editedWork);
}

AtomEditor::~AtomEditor(){
    AtomEditor::save();
    delete ui;
}

void AtomEditor::load(){
    QFile file{filePath()};
    if (!file.open(QIODevice::Truncate|QIODevice::ReadOnly|QIODevice::Text))
        return;
    //Load file
    QXmlStreamReader in{&file};
    if (!in.readNextStartElement()) return;
    if (in.name() == "ATOM"){
        ui->nameEditor->setText(in.attributes().value("NAME").toString());
        if (!in.readNextStartElement()) return;
        if (in.name() == "INPUT_PORTS"){
            readPorts(ui->inputEditor, in);
        }
        if (!in.readNextStartElement()) return;
        if (in.name() == "OUTPUT_PORTS"){
            readPorts(ui->outputEditor, in);
        }
        if (!in.readNextStartElement()) return;
        if (in.name() == "FUNCTION_BODY"){
            ui->codeEditor->setText(in.readElementText());
        }
    }
    file.close();
}

void AtomEditor::save(){
    QFile file{filePath()};
    if (!file.open(QIODevice::Truncate|QIODevice::WriteOnly|QIODevice::Text))
        return;
    QXmlStreamWriter out{&file};
    out.setAutoFormatting(true);
    out.setAutoFormattingIndent(2);
    out.writeStartDocument();
        out.writeStartElement("ATOM");
            out.writeAttribute("NAME", ui->nameEditor->text());
            out.writeStartElement("INPUT_PORTS");
                writePorts(ui->inputEditor->iterator(), out);
            out.writeEndElement();
            out.writeStartElement("OUTPUT_PORTS");
                writePorts(ui->outputEditor->iterator(), out);
            out.writeEndElement();
            out.writeStartElement("FUNCTION_BODY");
                out.writeCharacters(ui->codeEditor->toPlainText());
            out.writeEndElement();
        out.writeEndElement();
    out.writeEndDocument();
    file.close();
}

void AtomEditor::compile(){
    qDebug() << "comp";
}

void writePorts(QTreeWidgetItemIterator it, QXmlStreamWriter &out) {
    while (*it){
        out.writeTextElement((*it)->text(1), (*it)->text(0));
        ++it;
    }
}

void readPorts(PortEditor* port, QXmlStreamReader &in){
    while (in.readNextStartElement()){
        port->addPort(in.readElementText(), in.name().toString());
    }
}
