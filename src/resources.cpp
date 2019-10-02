/**
  \file
  \brief    Méthodes utilitaires permettant d'accéder aux ressources.
  \author   JCO
  \date     février 2014
  \version  1.0
*/
#include "resources.h"

#include <QCoreApplication>
#include <QDir>
#include <QDebug>

namespace GameFramework {
/**
Cette fonction retourne le chemin absolu du répertoire res.
Si la pseudo-constante DEPLOY est définie, elle se base sur la structure
de répertoires suivante :
\verbatim
+--ce_projet/
   |
   +--res/
   +ce_projet.exe
\endverbatim

Si la pseudo-constante DEPLOY n'est pas définie, elle se base sur la structure
de répertoires suivante sous Windows :
\verbatim
+--pendu/
   |
   +--build-ce_projet.../
   |  |
   |  +--debug/
   |  |  |
   |  |  +ce_projet.exe
   |  |
   |  +--release/
   |     |
   |     +ce_projet.exe
   |
   +--doc/
   +--res/
   +--src/
\endverbatim

Si la pseudo-constante DEPLOY n'est pas définie, elle se base sur la structure
de répertoires suivante sous Linux :
\verbatim
+--pendu/
   |
   +--build-ce_projet.../
   |  |
   |  +ce_projet.exe
   |
   +--doc/
   +--res/
   +--src/
\endverbatim

La pseudo-constante DEPLOY ne devrait être définie que lors de la génération de l'exécutable
en vue du déploiement.

Cette constante peut être définie au moment de la génération des MakeFile, avec la commande qmake.
Voici un exemple d'utilisation possible :

    qmake -config release "DEFINES+=DEPLOY" ..\..\src\GameFramwork.pro

Cette commande génère les fichiers MakeFile, nécessaires pour l'étape de compilation, en s'assurant
que la compilation se fasse en mode *Release* et que le pseudo-constante `DEPLOY` soit définie.


\return une chaîne de caractères contenant le chemin absolu du répertoire res.
*/
    QString resourcesPath() {
        QDir resourceDir = QDir(qApp->applicationDirPath());

        #ifndef DEPLOY
            #ifdef Q_OS_MAC
                resourceDir.cdUp(); // Quitte MacOS
                resourceDir.cdUp(); // Quitte Contents
                resourceDir.cdUp(); // Quitte GameFramwork.app
            #else
                #ifndef Q_OS_LINUX
                    resourceDir.cdUp(); // Quitte 'debug' ou 'release'
                #endif
            #endif
            resourceDir.cdUp(); // Quitte 'build...'
        #endif
        resourceDir.cd("res");
//        qDebug() << resourceDir.absolutePath();
        return resourceDir.absolutePath() + QDir::separator();
    }

/**
 * \return une chaîne de caractères contenant le chemin absolu du répertoire des images.
 */
    QString imagesPath() {
        return resourcesPath() /*+ QDir::separator()*/ + QString("images") + QDir::separator();
    }
}
