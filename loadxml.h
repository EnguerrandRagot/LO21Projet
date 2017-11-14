#ifndef LOADXML_H
#define LOADXML_H
#include "notes.h"
#include <set>

/****************************************************************************************************************************/
                                                /* XMLNOTEMANAGERREADER */
/****************************************************************************************************************************/
/** \class XmlNoteManagerReader
 * \brief Charge un NotesManager suivant un fichier XML
 */

class XmlNoteManagerReader
{
public:
    //! Un constructeur
    /** Ouvre le fichier et initialise le flux de lecture reader.
     */
    XmlNoteManagerReader() {
        //std::cout<<"Constructeur de XMLLOADER\n";
        QFile file(NotesManager::getManager().getFilename());
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            throw NoteException("Erreur ouverture en lecture fichier notes");
        }
        reader = new QXmlStreamReader(&file);
    }


    //! Fonction de lecture read
    /** \param Le fichier à lire, avec son mode d'ouverture.
     * Lit le fichier.
     */
    bool read(QIODevice *device);

private:
    QXmlStreamReader* reader;                                                           /**< pointeur sur le flux de lecture */
    std::set<QString> typedenote = {"Article", "Tache", "Video", "Audio", "Image"};     /**< Répertorie tous les types de NoteVersion*/


    //! Fonction de lecture readNotesManager
    /** Lit dans le fichier, ce qui se trouve dans le NotesManager.
     */
    void readNotesManager();


    //! Fonction de lecture readNote
    /** Lit dans le fichier, ce qui se trouve dans une Note et ajoute la Note correspondante au NotesManager.
     */
    void readNote();


    //! Fonction de lecture readRelation
    /** Lit dans le fichier, ce qui se trouve dans une Relation et ajoute la Relation correspondante au NotesManager.
     */
    void readRelation();


    //! Fonction de lecture readNoteVersion
    /** \param La Note dans laquelle est stockée la NoteVersion
     * Lit dans le fichier, ce qui se trouve dans une NoteVersion et ajoute la NoteVersion à la Note passée en paramètre.
     */
    void readNoteVersion(Note *n);


    //! Fonction de lecture readIdNote
    /** \param La Note dans laquelle sont stockés les attributs
     * Lit dans le fichier, les attributs d'une Note et les établit dans la Note passée en paramètre.
     */
    void readIdNote(Note *n);


    //! Fonction de lecture readCouple
    /** \param La Relation dans laquelle est stocké le Couple
     * Lit dans le fichier, ce qui se trouve dans un Couple et ajoute le Couple à la Relation passée en paramètre.
     */
    void readCouple(Relation *r);


    //! Fonction de lecture readAttributeRelation
    /** \param La Relation dans laquelle sont stocké les attributs
     * Lit dans le fichier, les attributs d'une Relation et les établit dans la Note passée en paramètre.
     */
    void readAttributeRelation(Relation *r);
};


#endif // LOADXML_H
