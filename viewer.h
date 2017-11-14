#ifndef VIEWER_H
#define VIEWER_H

/**
 * \file viewer.h
 * \brief Colections de widgets interagissant avec les notes
 * \version 0.1
 *
 * Ce fichier contient des widgets utiles pour manipuler les notes dans l'application.
 *
 */

#include <QtWidgets>

#include "notes.h"

class ArticleViewer;
class TacheViewer;
class MultimediaViewer;
class ImageViewer;
class VideoViewer;
class AudioViewer;

/*************************************************************************************************/
                                    /* NOTEVIEWER VISITOR */
/*************************************************************************************************/

/**
 * \class NoteViewerVisitor
 * \brief Visiteur de la classe Noteviewer
 *
 * Cette classe permet d'appliquer le desgin pattern visitor aux classes dépendantes de NoteViewer.
 *
 */

class NoteViewerVisitor {
public:

    /**
     * \brief Visit ArticleViewer
     *
     * Méthode appelée lorsqu'un ArticleViewer accepte un NoteViewerVisitor
     *
     * \param e : adresse d'un widget ArticleViewer
     *
     */

    virtual void visit(ArticleViewer *e) =0;

    /**
     * \brief Visit TacheViewer
     *
     * Méthode appelée lorsqu'un TacheViewer accepte un NoteViewerVisitor
     *
     * \param t : adresse d'un widget TacheViewer
     *
     */

    virtual void visit(TacheViewer *t) =0;

    /**
     * \brief Visit ImageViewer
     *
     * Méthode appelée lorsqu'un ImageViewer accepte un NoteViewerVisitor
     *
     * \param i : adresse d'un widget ImageViewer
     *
     */

    virtual void visit(ImageViewer *i) =0;

    /**
     * \brief Visit VideoViewer
     *
     * Méthode appelée lorsqu'un VideoViewer accepte un NoteViewerVisitor
     *
     * \param v : adresse d'un widget VideoViewer
     *
     */

    virtual void visit(VideoViewer *v) =0;

    /**
     * \brief Visit AudioViewer
     *
     * Méthode appelée lorsqu'un AudioViewer accepte un NoteViewerVisitor
     *
     * \param a : adresse d'un widget AudioViewer
     *
     */

    virtual void visit(AudioViewer* a) =0;
};

/*************************************************************************************************/
                                          /* NOTEVIEWER */
/*************************************************************************************************/

/**
 * \class NoteViewer
 * \brief Widget affichant une note
 *
 * Cette classe permet d'afficher la version d'une note donnée.
 *
 */

class NoteViewer : public QWidget {
    Q_OBJECT
    friend class Save; /* Save a besoin de récupérer les infos contenues dans les champs, pas forcément utile de passer
                          par des méthodes */
protected:
    /* Note */
    NoteVersion* note;
    /* Labels */
    QLabel *id_l;
    QLabel *title_l;
    QLabel *crea_l;
    QLabel *modif_l;
    /* Zones de saisies */
    QLineEdit *id_t;
    QLineEdit *title_t;
    QLineEdit *crea_t;
    QLineEdit *modif_t;
    /* Layouts */
    QHBoxLayout *id_hbox;
    QHBoxLayout *title_hbox;
    QHBoxLayout *crea_hbox;
    QHBoxLayout *modif_hbox;
    QVBoxLayout *fenetre_vbox;

public:

    /**
     * \brief Constructeur
     *
     * Construit un NoteViewer étant donné une NoteVersion
     *
     * \param n : référence sur une NoteVersion
     *
     */

    NoteViewer(NoteVersion& n);

    /**
     * \brief Destructeur
     *
     * Méthode appelée lors de la destruction d'un objet NoteViewer
     *
     */

    virtual ~NoteViewer() {}

    /**
     * \brief getCurrent
     *
     * Retourne la note affichée dans l'application.
     *
     * \return NoteVersion* : pointeur sur la NoteVersion affichée
     *
     */

    NoteVersion* getCurrentNote() { return note; }

    /**
     * \brief accept
     *
     * Accepte un NoteVisitor
     *
     * \param nv : référence sur NoteVisitor
     *
     */

    virtual void accept(class NoteViewerVisitor& nv) =0;
};

/*************************************************************************************************/
                                       /* ARTICLEVIEWER */
/*************************************************************************************************/

/**
 * \class ArticleViewer
 * \brief Widget affichant un article
 *
 * Cette classe permet d'afficher la version d'un article donné.
 *
 */

class ArticleViewer : public NoteViewer {
    Q_OBJECT
    friend class Save;

protected:
    /* Labels */
    QLabel *text_l;
    /* Zones de saisies */
    QTextEdit *text_t;
    /* Layouts */
    QHBoxLayout *text_hbox;

public:

    /**
     * \brief Constructeur
     *
     * Construit l'ArticleViewer.
     *
     * \param a : référence sur Article
     *
     */

    ArticleViewer(Article &a);

    /**
     * \brief accept
     *
     * Accepte un NoteVisitor
     *
     * \param nv : référence sur NoteVisitor
     *
     */

    void accept(NoteViewerVisitor& nv) { nv.visit(this); }
};

/*************************************************************************************************/
                                         /* TACHEVIEWER */
/*************************************************************************************************/

/**
 * \class TacheViewer
 * \brief Widget affichant une tache
 *
 * Cette classe permet d'afficher la version d'une tache donnée.
 *
 */

class TacheViewer : public NoteViewer {
    Q_OBJECT
    friend class Save;

protected:
    /* Labels */
    QLabel *action_l;
    QLabel *statut_l;
    QLabel *deadline_l;
    QLabel *prio_l;
    /* Zones de saisies */
    QTextEdit *action_t;
    QComboBox *statut_t;
    QCheckBox *deadline_c;
    QDateEdit *deadline_t;
    QCheckBox *prio_c;
    QSpinBox *prio_t;
    QLineEdit *prio_i;
    /* Layouts */
    QHBoxLayout *action_hbox;
    QHBoxLayout *statut_hbox;
    QHBoxLayout *deadline_hbox;
    QHBoxLayout *prio_hbox;

public:

    /**
     * \brief Constructeur
     *
     * Construit le TacheViewer
     *
     * \param t : référence sur Tache
     *
     */

    TacheViewer(Tache& t);

    /**
     * \brief accept
     *
     * Accepte un NoteVisitor
     *
     * \param nv : référence sur NoteVisitor
     *
     */

    void accept(NoteViewerVisitor& nv) { nv.visit(this); }
};

/*************************************************************************************************/
                                    /* MULTIMEDIAVIEWER */
/*************************************************************************************************/

/**
 * \class MultimediaViewer
 * \brief Widget affichant un contenu multimedia
 *
 * Cette classe permet d'afficher la version d'un multimedia donée.
 *
 */

class MultimediaViewer : public NoteViewer {
    Q_OBJECT
    friend class Save;

protected:
    /* Labels */
    QLabel *descr_l;
    QLabel *path_l;
    QLabel *img_l;
    /* Zones de saisies */
    QTextEdit *descr_t;
    QLineEdit *path_t;
    QLabel *img_t;
    /* Boutons */
    QPushButton *path_b;
    /* Layouts */
    QHBoxLayout *descr_hbox;
    QHBoxLayout *path_hbox;
    QHBoxLayout *img_hbox;

public:

    /**
     * \brief Constructeur
     *
     * Construit le MultimediaViewer
     *
     * \param m : référence sur Multimedia
     *
     */

    MultimediaViewer(Multimedia& m);

public slots:

    /**
     * \brief Charge une image
     *
     * Charge l'image, étant donné le chemin d'accès au fichier.
     *
     */

    void loadImage();
};

/**
 * \class ImageViewer
 * \brief Widget affichant une Image
 *
 * Cette classe permet d'afficher la version d'une Image donnée.
 *
 */

class ImageViewer : public MultimediaViewer {
    Q_OBJECT
    friend class Save;

public:

    /**
     * \brief Constructeur
     *
     * Construit l'ImageViewer.
     *
     * \param i : référence sur Image
     *
     */

    ImageViewer(Image& i);

    /**
     * \brief accept
     *
     * Accepte un NoteVisitor
     *
     * \param nv : référence sur NoteVisitor
     *
     */

    void accept(NoteViewerVisitor& nv) { nv.visit(this); }
};

/**
 * \class VideoViewer
 * \brief Widget affichant une video
 *
 * Cette classe permet d'afficher la version d'une Video donnée.
 *
 */

class VideoViewer : public MultimediaViewer {
    Q_OBJECT
    friend class Save;

public:

    /**
     * \brief Constructeur
     *
     * Construit le VideoViewer
     *
     * \param v : référence sur Video
     *
     */

    VideoViewer(Video& v);

    /**
     * \brief accept
     *
     * Accepte un NoteVisitor
     *
     * \param nv : référence sur NoteVisitor
     *
     */

    void accept(NoteViewerVisitor& nv) { nv.visit(this); }
};

/**
 * \class AudioViewer
 * \brief Widget affichant un Audio
 *
 * Cette classe permet d'afficher la version d'un Audio donné.
 *
 */

class AudioViewer : public MultimediaViewer {
    Q_OBJECT
    friend class Save;

public:

    /**
     * \brief Constructeur
     *
     * Construit un AudioViewer
     *
     * \param a : référence sur Audio
     *
     */

    AudioViewer(Audio& a);

    /**
     * \brief accept
     *
     * Accepte un NoteVisitor
     *
     * \param nv : référence sur NoteVisitor
     *
     */

    void accept(NoteViewerVisitor& nv) { nv.visit(this); }
};

/*************************************************************************************************/
                                       /* CENTRALZONE */
/*************************************************************************************************/

/**
 * \class CentralZone
 * \brief Conteneur pour le NoteViewer
 *
 * Ce widget sert à contenir un NoteViewer pour l'afficher.
 *
 */

class CentralZone : public QWidget {
    Q_OBJECT

private:
    NoteViewer* noteViewer;
    QLayout* layout;

public:

    /**
     * \brief Constructeur
     *
     * Construit le widget
     *
     */

    CentralZone();

    /**
     * \brief Récupère le NoteViewer affiché
     *
     * Cette fonction permet de récupérer l'adresse du widget NoteViewer contenu dans le CentralZone.
     *
     * \return NoteViewer* : pointeur sur le NoteViewer affiché
     *
     */

    NoteViewer* getNoteViewer() { return noteViewer; }

    /**
     * \brief Récupère la note affichée
     *
     * Cette fonction permet de récupérer l'adresse de la note affichée dans le NoteViewer.
     *
     * \return NoteVersion* : pointeur sur la version actuellement affichée de la note
     *
     */

    NoteVersion* getCurrentNote() { return noteViewer->getCurrentNote(); }

    /**
     * \brief Ajoute un NoteViewer
     *
     * Ajoute un NoteViewer à la CentralZone.
     *
     * \param nv : référence sur NoteViewer
     *
     */

    void addNoteViewer(NoteViewer *nv);

    /**
     * \brief Supprime un NoteViewer
     *
     * Retire le NoteViewer affiché.
     *
     */

    void removeNoteViewer();
};

#endif // VIEWER_H
