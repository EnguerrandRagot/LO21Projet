#include "loadxml.h"

bool XmlNoteManagerReader::read(QIODevice* device)
{
    //std::cout<<"XmlNoteManagerReader::read\n";
    reader->setDevice(device);
    //std::cout<<"XmlNoteManagerReader::read ; device set\n";

    if (reader->readNextStartElement()) {
        if (reader->name() == "NotesManager") readNotesManager();
        else throw NoteException("Ce n'est pas le NotesManager");
    }
    return !reader->error();
}


void XmlNoteManagerReader::readNotesManager(){
    //std::cout<<"XmlNoteManagerReader::readNotesManager\n";
    while(reader->readNextStartElement()){
        if(reader->name() == "Note") readNote();
        else if(reader->name() == "Relation") readRelation();
        else reader->skipCurrentElement();
    }
}


void XmlNoteManagerReader::readNote(){
    //std::cout<<"XmlNoteManagerReader::readNote\n";
    Q_ASSERT(reader->isStartElement() && reader->name() == "Note");

    Note* n = new Note();

    readIdNote(n);

    while (reader->readNextStartElement()) {
        //std::cout<<reader->name().toString().toStdString()<<"\n";
        if (typedenote.find(reader->name().toString()) != typedenote.end()) readNoteVersion(n);
        else reader->skipCurrentElement();
    }

    NotesManager::getManager().addNote(n);
}



void XmlNoteManagerReader::readRelation(){
    //std::cout<<"XmlNoteManagerReader::readRelation\n";
    Q_ASSERT(reader->isStartElement() && reader->name() == "Relation");

    QString titre = reader->attributes().value("Titre").toString();
    QString description = reader->attributes().value("Description").toString();

    NotesManager::getManager().addRelation(titre,description);

    Relation* r = NotesManager::getManager().getRelation(titre);

    while (reader->readNextStartElement()) {
        if (reader->name().toString()=="Couple") readCouple(r);
        else reader->skipCurrentElement();
    }

}




void XmlNoteManagerReader::readNoteVersion(Note* n){
    //std::cout<<"XmlNoteManagerReader::readNoteVersion\n";
    Q_ASSERT(reader->isStartElement() && typedenote.find(reader->name().toString()) != typedenote.end());

    NoteVersion* nv;

    do{
        if(typedenote.find(reader->name().toString()) != typedenote.end())
            nv = NoteVersion::CreateNote(reader);
        else reader->skipCurrentElement();
    } while (reader->readNextStartElement());
    if(nv)n->addNoteVersion(nv);
}



void XmlNoteManagerReader::readCouple(Relation* r){
    //std::cout<<"XmlNoteManagerReader::readNoteVersion\n";
    Q_ASSERT(reader->isStartElement() && reader->name().toString()=="Couple");

    do{
        if(reader->name().toString()=="Couple"){
            QString nx;
            QString ny;
            bool oriented;
            if (reader->attributes().hasAttribute("NX")) nx = reader->attributes().value("NX").toString();
            if (reader->attributes().hasAttribute("NY")) ny = reader->attributes().value("NY").toString();
            if (reader->attributes().hasAttribute("Oriented")) oriented = (bool)reader->attributes().value("Oriented").toInt();
            r->addCouple(nx,ny,oriented);
        }
        else reader->skipCurrentElement();
    } while (reader->readNextStartElement());
}



void XmlNoteManagerReader::readIdNote(Note* n){
    //std::cout<<"XmlNoteManagerReader::readIdNote\n";
    Q_ASSERT(reader->name() == "Note" && reader->attributes().hasAttribute("Id"));

    QString id = reader->attributes().value("Id").toString();
    n->setId(id);
}



