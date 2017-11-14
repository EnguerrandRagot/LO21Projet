#ifndef COMMAND_H
#define COMMAND_H

/**
 * \file command.h
 * \brief Collections de Visitors
 * \version 0.1
 *
 * Ce fichier contient plusieurs Visitors utilisés dans l'application.
 *
 */

#include "viewer.h"
#include "mainwindow.h"
#include "notes.h"

/*************************************************************************************************/
                                            /* VIEW */
/*************************************************************************************************/
/**
 *
 * \class View
 * \brief Permet de lancer le Viewer correspondant à la NoteVersion.
 */

class View : public NoteVisitor {
public:

    /**
     * \brief Visit Article
     *
     * Méthode appelée lorsqu'un Article accepte un NoteVisitor View
     *
     * \param n : adresse d'un Article
     *
     */

    void visit(Article* n);

    /**
     * \brief Visit Tache
     *
     * Méthode appelée lorsqu'une Tache accepte un NoteVisitor View
     *
     * \param n : adresse d'une Tache
     *
     */

    void visit(Tache *n);

    /**
     * \brief Visit Video
     *
     * Méthode appelée lorsqu'une Video accepte un NoteVisitor View
     *
     * \param n : adresse d'une Video
     *
     */

    void visit(Video *n);

    /**
     * \brief Visit Image
     *
     * Méthode appelée lorsqu'une Image accepte un NoteVisitor View
     *
     * \param n : adresse d'une Image
     *
     */

    void visit(Image *n);

    /**
     * \brief Visit Audio
     *
     * Méthode appelée lorsqu'un Audio accepte un NoteVisitor View
     *
     * \param n : adresse d'un Audio
     *
     */

    void visit(Audio *n);
};


/*************************************************************************************************/
                                            /* SAVEXML */
/*************************************************************************************************/
/**
 * \class SaveXML
 *
 * \brief Hérite de NoteVisitor. Permet de sauvegarder les NoteVersions dans le fichier XML.
 *
 */

class SaveXML : public NoteVisitor {
public:
    void visit(Article *n);
    void visit(Tache *n);
    void visit(Video *n);
    void visit(Image *n);
    void visit(Audio *n);
};



/*************************************************************************************************/
                                            /* SAVE */
/*************************************************************************************************/

/**
 * \class Save
 *
 * \brief Hérite de NoteViewerVisitor. Permet de sauvegarder les données des Viewer dans les NoteVersion.
 *
 */

class Save : public NoteViewerVisitor {
public:

    /**
     * \brief Visit ArticleViewer
     *
     * Méthode appelée lorsqu'un ArticleViewer accepte un NoteViewerVisitor Save
     *
     * \param v : adresse d'un widget ArticleViewer
     *
     */

    void visit(ArticleViewer* v);

    /**
     * \brief Visit TacheViewer
     *
     * Méthode appelée lorsqu'un TacheViewer accepte un NoteViewerVisitor Save
     *
     * \param v : adresse d'un widget TacheViewer
     *
     */

    void visit(TacheViewer* v);

    /**
     * \brief Visit ImageViewer
     *
     * Méthode appelée lorsqu'un ImageViewer accepte un NoteViewerVisitor Save
     *
     * \param v : adresse d'un widget ImageViewer
     *
     */

    void visit(ImageViewer* v);

    /**
     * \brief Visit VideoViewer
     *
     * Méthode appelée lorsqu'un VideoViewer accepte un NoteViewerVisitor Save
     *
     * \param v : adresse d'un widget VideoViewer
     *
     */

    void visit(VideoViewer* v);

    /**
     * \brief Visit AudioViewer
     *
     * Méthode appelée lorsqu'un AudioViewer accepte un NoteViewerVisitor Save
     *
     * \param v : adresse d'un widget AudioViewer
     *
     */

    void visit(AudioViewer* v);
};

#endif // COMMAND_H
