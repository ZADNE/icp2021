/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#include "atomeditor.h"
#include "ui_atomeditor.h"

#include <QDebug>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "blockcompiler.h"

//Helper functions
void writeVariablesToXML(VariableEditor* ve, QXmlStreamWriter &out);
void readVariablesFromXML(VariableEditor* ve, QXmlStreamReader &in);

AtomEditor::AtomEditor(QWidget *parent):
    BlockEditor(parent),
    ui(new Ui::AtomEditor)
{
    ui->setupUi(this);

    connect(ui->inputEditor, &VariableEditor::edited,
            this, &AtomEditor::editedWork);
    connect(ui->outputEditor, &VariableEditor::edited,
            this, &AtomEditor::editedWork);
    connect(ui->codeEditor, &QTextEdit::textChanged,
            this, &AtomEditor::editedWork);
    connect(ui->stateVarEditor, &QTextEdit::textChanged,
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
            readVariablesFromXML(ui->inputEditor, in);
        }
        if (!in.readNextStartElement()) return;
        if (in.name() == "OUTPUT_PORTS"){
            readVariablesFromXML(ui->outputEditor, in);
        }
        if (!in.readNextStartElement()) return;
        if (in.name() == "FUNCTION_BODY"){
            ui->codeEditor->setText(in.readElementText());
        }
        if (!in.readNextStartElement()) return;
        if (in.name() == "STATE_VARIABLES"){
            ui->stateVarEditor->setText(in.readElementText());
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
                writeVariablesToXML(ui->inputEditor, out);
            out.writeEndElement();
            out.writeStartElement("OUTPUT_PORTS");
                writeVariablesToXML(ui->outputEditor, out);
            out.writeEndElement();
            out.writeStartElement("FUNCTION_BODY");
                out.writeCharacters(ui->codeEditor->toPlainText());
            out.writeEndElement();
            out.writeStartElement("STATE_VARIABLES");
                out.writeCharacters(ui->stateVarEditor->toPlainText());
            out.writeEndElement();
        out.writeEndElement();
    out.writeEndDocument();
    file.close();
}

void AtomEditor::build(){
    BlockCompiler::bc().buildAtom(filePath().toStdString());
}

void writeVariablesToXML(VariableEditor* ve, QXmlStreamWriter &out) {
    auto it = ve->iterator();
    while (*it){
        out.writeStartElement((*it)->text(1));
        out.writeAttribute("TEMPLATE", ((*it)->checkState(0) == Qt::Checked ? "TRUE" : "FALSE"));
        out.writeCharacters((*it)->text(2));
        out.writeEndElement();
        ++it;
    }
}

void readVariablesFromXML(VariableEditor* ve, QXmlStreamReader &in){
    while (in.readNextStartElement()){
        bool templ = false;
        if (in.attributes().value("TEMPLATE") == "TRUE"){
            templ = true;
        }
        ve->addVariable(templ, in.name().toString(), in.readElementText());
    }
}
