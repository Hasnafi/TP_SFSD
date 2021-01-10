#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <math.h>

#define Max 1500
#define Max_Index 200

/// **************************************** DECLARATION DES STRUCTURES,TYPES ET CONSTANTES  ********************************************* ///

/// Les structures du fichier LNOF ///

//---------------------------------------------//

typedef struct Tenreg
{
    long Matricule;
    char Nom[50];
    char Prenom[50];
    char Date_Naissance[50];
    char Wilaya_Naissance[50];
    char Groupe_Sanguin[10];
    char Grade[40];
    char Force_Armee[40];
    char Region_Militaire[40];

} Tenreg;


//---------------------------------------------//

typedef struct TBloc
{
    Tenreg Tab[Max];
    int nb; // nombre d'enregistrement dans le bloc
    int suivant;

} TBloc;

//---------------------------------------------//

typedef struct TBloc Buffer;

//---------------------------------------------//

typedef struct Entete
{
    int nbBloc;
    int tete; // numéro du premier bloc de la liste
    int queue; // numéro du dernier bloc de la liste
    int nbEnreg;

} Entete;

//---------------------------------------------//

typedef struct LNOF
{
    FILE *fichier;
    Entete entete;

} LNOF;

//---------------------------------------------//


/// Les structures du fichier Index TOF ///

//---------------------------------------------//

typedef struct entete_index
{
    int nb_bloc;

} entete_index;

//---------------------------------------------//

typedef struct TOF_index
{
    FILE *fichier;
    entete_index entete;

}TOF_index;

//---------------------------------------------//

typedef struct index
{
    long int Matricule;
    int i;
    int j;
    int effacement; // 1 si l'index est efface 0 sinon

}index;

//---------------------------------------------//

typedef struct blocIndex
{
    index tab[Max_Index];
    int nb_Enreg;

}blocIndex;

//---------------------------------------------//

typedef struct blocIndex BufferIndex;

//---------------------------------------------//

typedef struct indexGategorieGrade
{
    int NumCategorie;
    int i;
    int j;
    int effacement; // 1 si l'index est efface 0 sinon

}indexGategorieGrade;

//---------------------------------------------//

typedef struct blocIndexGrade
{
    indexGategorieGrade tab[Max_Index];
    int nb_Enreg;

}blocIndexGrade;

//---------------------------------------------//

typedef struct blocIndexGrade BufferIndexGrade;

//---------------------------------------------//


typedef enum
{
    BLACK,          /**< black color */
    BLUE,           /**< blue color */
    GREEN,          /**< green color */
    CYAN,           /**< cyan color */
    RED,            /**< red color */
    MAGENTA,        /**< magenta color */
    BROWN,          /**< brown color */
    LIGHTGRAY,      /**< light gray color */
    DARKGRAY,       /**< dark gray color */
    LIGHTBLUE,      /**< light blue color */
    LIGHTGREEN,     /**< light green color */
    LIGHTCYAN,      /**< light cyan color */
    LIGHTRED,       /**< light red color */
    LIGHTMAGENTA,   /**< light magenta color */
    YELLOW,         /**< yellow color */
    WHITE           /**< white color */
} COLORS;



/// Les constantes ///

const char Wilayas[][20] =
{
    "ADRAR", "CHLEF", "LAGHOUAT", "OUM BOUAGHI", "BATNA",
    "BEJAIA", "BISKRA", "BECHAR", "BLIDA", "BOUIRA", "TAMANRASSET",
    "TEBESSA", "TLEMCEN", "TIARET", "TIZI OUZOU", "ALGER", "DJELFA",
    "JIJEL", "SETIF", "SAIDA","SKIKDA", "SIDI BEL ABBES", "ANNABA",
    "GUELMA", "CONSTANTINE", "MEDEA", "MOSTAGANEM", "M'SILA","MASCARA",
    "OUARGLA", "ORAN", "EL BAYDH", "ILLIZI", "BORDJ BOU ARERIDJ",
    "BOUMERDES", "EL TAREF","TINDOUF", "TISSEMSILT", "EL OUED",
    "KHENCHLA", "SOUK AHRASS", "TIPAZA", "MILA", "AIN DEFLA", "NAAMA",
    "AIN TEMOUCHENT", "GHARDAÏA", "RELIZANE", "EL M'GHAIR", "EL MENIA",
    "OULED DJELLAL", "BORJ BAJI MOKHTAR","BENI ABBES", "TIMIMOUN",
    "TOUGGOURT", "DJANET", "IN SALAH", "IN GUEZZAM"
};

//---------------------------------------------//

const char Grade[][40] =
{
    "General-Major",
    "General",
    "Colonel",
    "Lieutenant-colonel",
    "Commandant",
    "Capitaine",
    "Lieutenant",
    "Sous-lieutenant",
    "Aspirant",
    "Adjudant-chef",
    "Adjudant",
    "Sergent-chef",
    "Sergent",
    "Caporal-chef",
    "Caporal",
    "Djoundi"
};

//---------------------------------------------//

const char RegionMilitaire[][40] =
{
    "1RM-Blida",
    "2RM-Oran",
    "3RM-Bechar",
    "4RM-Ouargla",
    "5RM-Constantine",
    "6RM-Tamanrasset"
};

//---------------------------------------------//

const char groupe_sanguin[][40] =
{
    "O+",
    "A+",
    "B+",
    "O-",
    "A-",
    "AB+",
    "B-",
    "AB-"
};

const char force_armee[][40] =
{
    "Armee_de_terre", "Armee_de_l_air", "Marine_nationale",
    "Defense_aerienne_du_territoire","Gendarmerie_nationale",
    "Garde_republicaine", "Departement_du_renseignement_et_de_la_securite",
    "Sante_militaire"
};

const char categorie[][40] =
{
    "Officiers-généraux", "Officiers-supérieurs",
    "Officiers", "Sous-officiers", "Hommes de troupes"
};

/// ******************************************************* LES MODELES DE FICHIER TOF ******************************************************* ///
                                                           // Pour le fichier index //


/// -------------------------------------------- L'ouverture du fichier TOF ------------------------------------------------- //
int nb_l=0,nb_e=0;
int ouvrir2(TOF_index *f,char *chemin,char mode)
{
    f = malloc(sizeof(TOF_index));
    char s[3];

    if((mode == 'A') || (mode == 'a')) sprintf(s,"rb+");
    else if ((mode == 'N') || (mode == 'n')) sprintf(s,"wb+");
    else return 1;

    (f->fichier) = fopen(chemin, s);
    if(f->fichier == NULL)
    {
        printf("IMPOSSIBLE D'OUVRIR LE FICHIER ! ");
    }
    if((mode == 'A') || (mode == 'a'))
    {
        fread(&(f->entete), sizeof(entete_index), 1, f->fichier);
    }
    else if ((mode == 'N') || (mode == 'n'))
    {
        f->entete.nb_bloc = 0;
    }
    return 0;
}



/// -------------------------------------------- La fermeture du fichier TOF ------------------------------------------------- //


void fermer2(TOF_index *f)
{
    rewind(f->fichier);
    fwrite(&(f->entete), sizeof(entete_index), 1, f->fichier);
    fclose(f->fichier);
    free(f);
}


/// -------------------------------------------- L'entete du fichier TOF ------------------------------------------------- //


int entete2(TOF_index *f,int i)
{
    if(i == 1) return (f->entete).nb_bloc;
         else return -1;
}


/// -------------------------------------------- L'entete du fichier TOF ------------------------------------------------- //


void aff_entete2(TOF_index *f, int i, int val)
{
    if(i == 1) (f->entete).nb_bloc = val;

    else {printf ("caracteristique inexistante ! ");}
}


/// -------------------------------------------- Alloc bloc d'un fichier TOF ------------------------------------------------- //


int alloc_bloc2(TOF_index *f)
{
    aff_entete2(f,1,entete2(f,1)+1);
    return entete2(f,1);
}


/// -------------------------------------------- La lecture dans un fichier TOF ------------------------------------------------- //


void lireDir2(TOF_index *f,int Num_Bloc,blocIndex *buffer)
{
    if(Num_Bloc <= (f->entete).nb_bloc)
    {
        fseek(f->fichier, sizeof(entete_index) + (Num_Bloc-1) * sizeof(blocIndex),SEEK_SET);
        fread(buffer,  sizeof(blocIndex), 1,f->fichier);
    }
}


/// -------------------------------------------- L'ecriture dans un fichier TOF ------------------------------------------------- //


void ecrireDir2(TOF_index *f, int Num_Bloc,blocIndex *buf)
{
    if(Num_Bloc <= (f->entete).nb_bloc)
    {
        fseek(f->fichier, sizeof(entete_index) + (Num_Bloc-1) * sizeof(blocIndex), SEEK_SET);
        fwrite(buf, 1, sizeof(blocIndex), f->fichier);
    }
}


/// ******************************************************* LES MODELES DE L/OF ******************************************************* ///

/// -------------------------------------------- L'ouverture du fichier L/OF ------------------------------------------------- //

LNOF* Ouvrir(const char nomFichier[], char mode)
{

    LNOF* f = malloc(sizeof(LNOF));
    if((mode == 'A') || (mode == 'a'))
    {
        f->fichier = fopen(nomFichier, "rb+");
        if(f->fichier == NULL)
            return NULL;
        else
            fread(&(f->entete), sizeof(Entete), 1, f->fichier);
    }
    else if((mode == 'N') || (mode == 'n'))
    {
        f->fichier = fopen(nomFichier, "wb+");
        if(f->fichier == NULL)
            return NULL;
        else
        {
            f->entete.nbBloc = 0;
            f->entete.nbEnreg = 0;
        }
    } else return NULL;
    return f;
}

/// ----------------------------------------------- La fermeture d'un fichier L/OF ---------------------------------------------- //

void Fermer(LNOF *f)
{
    rewind(f->fichier);
    fwrite(&(f->entete), sizeof(Entete),1, f->fichier);
    fclose(f->fichier);
    free(f);
}

/// ----------------------------------------------- L'ecriture dans le fichier L/OF --------------------------------------------- //

void lireDir(LNOF *f, int Num_Bloc, Buffer *buf)
{
    if(Num_Bloc <= (f->entete).nbBloc)
    {
        fseek(f->fichier, sizeof(Entete) + (Num_Bloc-1) * sizeof(TBloc), SEEK_SET);
        fread(buf, 1, sizeof(TBloc), f->fichier);
    }
}

/// ------------------------------------------------ La lecture dans le fichier L/OF --------------------------------------------- //

void ecrireDir(LNOF *f, int Num_Bloc, Buffer *buf)
{
    if(Num_Bloc <= (f->entete).nbBloc)
    {
        fseek(f->fichier, sizeof(Entete) + (Num_Bloc-1) * sizeof(TBloc), SEEK_SET);
        fwrite(buf, 1, sizeof(TBloc), f->fichier);
    }
}

/// --------------------------------------- Le remplissage de l'entete du fichier L/OF  ------------------------------------------ //

void aff_entete(LNOF *f, int i , int val)
{
    switch(i)
    {
        case 1:
            f->entete.nbBloc=val; // nombre total de bloc alloués dans le fichier
            break;
         case 2:
            f->entete.tete=val;  // numéro du bloc représentatnt la tête du fichier
            break;
         case 3:
            f->entete.queue=val;  // numéro du bloc representatnt la tête du fichier
            break;
         case 4:
            f->entete.nbEnreg=val;  // nombre d'enregistrements non éffacés dans le fichier
            break;
    }
}

/// -------------------------------------------------- L'entete du fichier L/OF  ------------------------------------------------- //

int entete(LNOF *f, int i)
{
    int champ;
    switch(i)
    {
        case 1:
            champ = (f->entete.nbBloc);  // nombre total de bloc alloués dans le fichier
            break;
         case 2:
            champ = (f->entete.tete);   // numéro du bloc représentatnt la tête du fichier
            break;
         case 3:
            champ = (f->entete.queue);   // numéro du bloc representatnt la tête du fichier
            break;
         case 4:
            champ = (f->entete.nbEnreg);  // nombre d'enregistrements dans le fichier
            break;
    }
    return champ;
}

/// -------------------------------------------- Alloc bloc d'un fichier L/OF ------------------------------------------------- //


int alloc_bloc(LNOF *f)
{
    aff_entete(f,1,entete(f,1)+1);
    return entete(f,1);
}

/// ********************************** Les fonctions et procedures de fichier INDEX ********************************** ///

/// ***************************************** RECHERCHE_index ***************************************** ///

void rechDicho_TOF(long mat, TOF_index *f, int* trouv,int* i,int* j)
{
    BufferIndex buf;
    *trouv = 0;
    int stop = 0;
    int infB, supB;
    int infE, supE;
    if (entete2(f,1) == 0) // en cas le fichier est vide
    {
        *i = 1;
        *j = 0;
        return;
    }
    /* Recherche externe */
    infB = 1; // le premier bloc
    supB = entete2(f, 1); //le dernier bloc
    while(infB<=supB && !(*trouv) && !stop)
    {
        *i = (infB + supB) / 2; // le numéro du bloc du milieu
        lireDir2(f, *i, &buf);
        if (buf.tab[0].Matricule <= mat && mat <= buf.tab[buf.nb_Enreg-1].Matricule)
        {
            /* Recherche interne */
            infE = 0; // le premier enregistrement du bloc
            supE = buf.nb_Enreg - 1; // le dernier enregistrement du bloc
            while(infE<=supE && !(*trouv))
            {
                *j = (infE + supE) / 2; // l'indice de l'enregistrement du milieu
                if (buf.tab[*j].Matricule == mat)
                {
                  if ( buf.tab[*j].effacement == 0)
                    *trouv = 1;
                  else
                    stop = 1;
                }
                else
                {
                    if (buf.tab[*j].Matricule> mat)
                        supE = *j - 1;
                    if (buf.tab[*j].Matricule < mat)
                        infE = *j + 1;
                }
            }
            if (infE>supE) // le numéro recherché n'existe pas dans le bloc
                *j = infE; // la position ou il faut l'inséerer alors
            stop = 1;
            /* Fin de la recherche interne */
        }
        else
        {
            if (buf.tab[0].Matricule > mat)
                supB = *i - 1;
            if (mat > buf.tab[buf.nb_Enreg-1].Matricule)
                infB = *i + 1;
        }
    }
    if (infB>supB && mat<buf.tab[0].Matricule)
        *j = 0;
    if (infB>supB && mat>buf.tab[buf.nb_Enreg-1].Matricule) // le numéro recherché est plus grand que
    {
        if (buf.nb_Enreg == Max_Index) // le dernier bloc est plein
        {
            *i = infB; // il faut insérer l'enregistrement dans un nouveau bloc
            *j = 0;
        }
        else // le dernier bloc n'est pas encore plein
            *j = buf.nb_Enreg; // il faut l'insérer dans le dernier bloc à la première position libre
    }
}

/// ***************************************** INSERTION_index ***************************************** ///

int insertion_TOF (TOF_index* f, index enreg)
{
    int trouv,i,j;
    rechDicho_TOF(enreg.Matricule,f,&trouv,&i,&j);
    if (trouv) // le numéro de CCP existe déja dans le fichier et on ne peut pas le réinsérer
        return 0;
    else // le numéro de CCP n'existe pas dans le fichier, il faut l'insérer à l'emplacement (i,j)
    {
        BufferIndex buf;
        index x;
        int k,continu=1;
        while ((continu==1) && (i<=entete2(f,1)))
        {
            lireDir2(f,i,&buf);
            x = buf.tab[buf.nb_Enreg-1]; // sauvegarder le dernier enregistrement du bloc
            /* Décalages intra-bloc */
            for(k = buf.nb_Enreg-1; k>j; k--)
                buf.tab[k] = buf.tab[k-1];
            buf.tab[j] = enreg;
            if(buf.nb_Enreg < Max_Index) // si le bloc n'est pas plein, on remet x dans permier indice libre du bloc
            {
                buf.nb_Enreg++;
                if (j == buf.nb_Enreg-1) // le nouvel enregistrement est le dernier enregistrement du fichier
                    buf.tab[buf.nb_Enreg-2] = x;
                else // le nouvel enregistrement a été inséré entre deux enregistrements
                    buf.tab[buf.nb_Enreg-1] = x;
                ecrireDir2(f,i,&buf);
                continu = 0;
            }
            else // si le bloc est plein, x doit être inséré dans un nouveau bloc i+1 à la position 0
            {
                ecrireDir2(f,i,&buf);
                i++;
                j = 0;
                enreg = x; // l'enregistrement qui doit être inséré à l'emplacement (i,0)
            }
        }
        // si on dépasse la fin de fichier, on rajoute un nouveau bloc contenant un seul enregistrement
        if (i>entete2(f,1))
        {
            buf.tab[0] = enreg;
            buf.nb_Enreg = 1;
           // alloc_bloc2(f);
            ecrireDir2(f,alloc_bloc2(f),&buf);
        }
        return 1;
    }
}

/// ***************************************** RECHERCHE_index2 ***************************************** ///

void rechDicho2_TOF(int mat, TOF_index *f, int* trouv,int* i,int* j)
{
    BufferIndexGrade buf;
    *trouv = 0;
    int stop = 0;
    int infB, supB;
    int infE, supE;
    if (entete2(f,1) == 0) // en cas le fichier est vide
    {
        *i = 1;
        *j = 0;
        return;
    }
    /* Recherche externe */
    infB = 1; // le premier bloc
    supB = entete2(f, 1); //le dernier bloc
    while(infB<=supB && !(*trouv) && !stop)
    {
        *i = (infB + supB) / 2; // le numéro du bloc du milieu
        lireDir2(f, *i, &buf);
        if (buf.tab[0].NumCategorie <= mat && mat <= buf.tab[buf.nb_Enreg-1].NumCategorie)
        {
            /* Recherche interne */
            infE = 0; // le premier enregistrement du bloc
            supE = buf.nb_Enreg - 1; // le dernier enregistrement du bloc
            while(infE<=supE && !(*trouv))
            {
                *j = (infE + supE) / 2; // l'indice de l'enregistrement du milieu
                if (buf.tab[*j].NumCategorie == mat)
                    *trouv = 1;
                else
                {
                    if (buf.tab[*j].NumCategorie> mat)
                        supE = *j - 1;
                    if (buf.tab[*j].NumCategorie < mat)
                        infE = *j + 1;
                }
            }
            if (infE>supE) // le numéro recherché n'existe pas dans le bloc
                *j = infE; // la position ou il faut l'inséerer alors
            stop = 1;
            /* Fin de la recherche interne */
        }
        else
        {
            if (buf.tab[0].NumCategorie > mat)
                supB = *i - 1;
            if (mat > buf.tab[buf.nb_Enreg-1].NumCategorie)
                infB = *i + 1;
        }
    }
    if (infB>supB && mat<buf.tab[0].NumCategorie)
        *j = 0;
    if (infB>supB && mat>buf.tab[buf.nb_Enreg-1].NumCategorie) // le numéro recherché est plus grand que
    {
        if (buf.nb_Enreg == Max_Index) // le dernier bloc est plein
        {
            *i = infB; // il faut insérer l'enregistrement dans un nouveau bloc
            *j = 0;
        }
        else // le dernier bloc n'est pas encore plein
            *j = buf.nb_Enreg; // il faut l'insérer dans le dernier bloc à la première position libre
    }
}

/// ***************************************** INSERTION_index2 ***************************************** ///

int insertion2_TOF (TOF_index* f, indexGategorieGrade enreg)
{
    int i,j,trouv;
    rechDicho2_TOF(enreg.NumCategorie,f,&trouv,&i,&j);
    BufferIndexGrade buf;
    indexGategorieGrade x;
    int k,continu=1;
    while ((continu==1) && (i<=entete2(f,1)))
    {
        lireDir2(f,i,&buf);
        x = buf.tab[buf.nb_Enreg-1]; // sauvegarder le dernier enregistrement du bloc
        /* Décalages intra-bloc */
        for(k = buf.nb_Enreg-1; k>j; k--)
            buf.tab[k] = buf.tab[k-1];
        buf.tab[j] = enreg;
        if(buf.nb_Enreg < Max_Index) // si le bloc n'est pas plein, on remet x dans permier indice libre du bloc
        {
            buf.nb_Enreg++;
            if (j == buf.nb_Enreg-1) // le nouvel enregistrement est le dernier enregistrement du fichier
                buf.tab[buf.nb_Enreg-2] = x;
            else // le nouvel enregistrement a été inséré entre deux enregistrements
                buf.tab[buf.nb_Enreg-1] = x;
            ecrireDir2(f,i,&buf);
            continu = 0;
        }
        else // si le bloc est plein, x doit être inséré dans un nouveau bloc i+1 à la position 0
        {
            ecrireDir2(f,i,&buf);
            i++;
            j = 0;
            enreg = x; // l'enregistrement qui doit être inséré à l'emplacement (i,0)
        }
    }
    // si on dépasse la fin de fichier, on rajoute un nouveau bloc contenant un seul enregistrement
    if (i>entete2(f,1))
    {
        buf.tab[0] = enreg;
        buf.nb_Enreg = 1;
        // alloc_bloc2(f);
        ecrireDir2(f,alloc_bloc2(f),&buf);
    }
    return 1;
}

/// ***************************************** CREATION_index ***************************************** ///

TOF_index *creatiaonIndex(TOF_index *f, LNOF *fichier, char nomfich[30])   /** on va creer ce fichier a partir du fichier L/OVF **/
{
    int i,j,nbEnreg,nbBloc,k,nb;
    int trouv2,bloc,depl;
    long matricule;
    Buffer buffer1;
    BufferIndex buf;
    index enreg;
    f = Ouvrir("index.bin",'N');
    nbBloc = (fichier->entete).nbBloc; // pour recuperer les matricules du fichier LNOF
    nb = (fichier->entete).nbEnreg;
    k = 0;
    for (i=1; i<= nbBloc; i++)
    {
        lireDir(fichier,i,&buffer1);
        nb_l++;
        nbEnreg = buffer1.nb;
        for (j=0; j<nbEnreg; j++)
        {
            enreg.Matricule = buffer1.Tab[j].Matricule;
            enreg.i = i;
            enreg.j = j;
            enreg.effacement = 0;
            insertion_TOF(f,enreg);
            gotoxy(10,10);
            printf("\n\t\t\t\t\tNombre d'enregistrement remplie : %d/%d",k+1,nb);
            printf("\n\n\t\t\t\t\tChargement ... %d \%",(k+1)*100/nb);
            gotoxy(10,10);
            k = k+1;
        }
    }
    return f;
}

/// ******************************** CREATION_index_categorie_grade *********************************** ///

TOF_index *creaIndexGrade(TOF_index *f, LNOF *fichier, char nomfich[30])
{
    int i,j,nbEnreg,nbBloc,k,nb;
    int trouv2,bloc,depl;
    int numeroCategorie;
    Buffer buffer1;
    BufferIndexGrade buf;
    indexGategorieGrade enreg;
    f = Ouvrir("indexGrade.bin",'N');
    nb = (fichier->entete).nbEnreg;
    k = 0;
    nbBloc = (fichier->entete).nbBloc; // pour recuperer les grades du fichier LNOF
    for (i=1; i<= nbBloc; i++)
    {
        lireDir(fichier,i,&buffer1);
        nb_l++;
        nbEnreg = buffer1.nb;
        for (j=0; j<nbEnreg; j++)
        {
            if ( (strcmp(buffer1.Tab[j].Grade,"General de corps d'armee") == 0 ) || (strcmp(buffer1.Tab[j].Grade,"General-Major") == 0) || (strcmp(buffer1.Tab[j].Grade,"General")== 0) )
            {
                enreg.NumCategorie = 1;
            }
            if ( (strcmp(buffer1.Tab[j].Grade,"Colonel") == 0 ) || ( strcmp(buffer1.Tab[j].Grade,"Lieutenant-colonel") == 0) || ( strcmp(buffer1.Tab[j].Grade,"Commandant")== 0) )
            {
                enreg.NumCategorie = 2;
            }
            if ( ( strcmp(buffer1.Tab[j].Grade,"Capitaine") == 0) || (strcmp(buffer1.Tab[j].Grade,"Lieutenant") ==0)|| (strcmp(buffer1.Tab[j].Grade,"Sous-lieutenant")==0) || (strcmp(buffer1.Tab[j].Grade,"Aspirant") == 0) )
            {
                enreg.NumCategorie = 3;
            }
            if ( (strcmp(buffer1.Tab[j].Grade,"Adjudant-chef") == 0) || (strcmp(buffer1.Tab[j].Grade,"Adjudant")==0) || (strcmp(buffer1.Tab[j].Grade,"Sergent-chef")==0 )|| (strcmp(buffer1.Tab[j].Grade,"Sergent")==0))
            {
                enreg.NumCategorie = 4;
            }
            if ( (strcmp(buffer1.Tab[j].Grade,"Caporal-chef")==0)||(strcmp(buffer1.Tab[j].Grade,"Caporal")==0) || (strcmp(buffer1.Tab[j].Grade,"Djoundi")==0))
            {
                enreg.NumCategorie = 5;
            }
            enreg.i = i;
            enreg.j = j;
            enreg.effacement = 0;
            insertion2_TOF(f,enreg);
            gotoxy(10,10);
            printf("\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tNombre d'enregistrement remplie : %d/%d",k+1,nb);
            printf("\n\n\t\t\t\t\tChargement ... %d \%",(k+1)*100/nb);
            gotoxy(10,10);
            k = k+1;
        }
    }
    return f;
}
///*********************************************Index_force********************************************///
//---------------------------------------------//

typedef struct indexGategorieForce
{
    int NumCategorie;
    int i;
    int j;
    int effacement; // 1 si l'index est efface 0 sinon

}indexGategorieForce;

//---------------------------------------------//

typedef struct blocIndexForce
{
    indexGategorieForce tab[Max_Index];
    int nb_Enreg;

}blocIndexForce;

//---------------------------------------------//

typedef struct blocIndexForce BufferIndexForce;

//---------------------------------------------//


/// ***************************************** RECHERCHE_index3 ***************************************** ///

void rechDicho_force_TOF(int mat, TOF_index *f, int* trouv,int* i,int* j)
{
    BufferIndexForce buf;
    *trouv = 0;
    int stop = 0;
    int infB, supB;
    int infE, supE;
    if (entete2(f,1) == 0) // en cas le fichier est vide
    {
        *i = 1;
        *j = 0;
        return;
    }
    /* Recherche externe */
    infB = 1; // le premier bloc
    supB = entete2(f, 1); //le dernier bloc
    while(infB<=supB && !(*trouv) && !stop)
    {
        *i = (infB + supB) / 2; // le numéro du bloc du milieu
        lireDir2(f, *i, &buf);
        if (buf.tab[0].NumCategorie <= mat && mat <= buf.tab[buf.nb_Enreg-1].NumCategorie)
        {
            /* Recherche interne */
            infE = 0; // le premier enregistrement du bloc
            supE = buf.nb_Enreg - 1; // le dernier enregistrement du bloc
            while(infE<=supE && !(*trouv))
            {
                *j = (infE + supE) / 2; // l'indice de l'enregistrement du milieu
                if (buf.tab[*j].NumCategorie == mat){
                        if ( buf.tab[*j].effacement == 0)
                          *trouv = 1;
                       else
                        stop = 1;
                }
                else
                {
                    if (buf.tab[*j].NumCategorie> mat)
                        supE = *j - 1;
                    if (buf.tab[*j].NumCategorie < mat)
                        infE = *j + 1;
                }
            }
            if (infE>supE) // le numéro recherché n'existe pas dans le bloc
                *j = infE; // la position ou il faut l'inséerer alors
            stop = 1;
            /* Fin de la recherche interne */
        }
        else
        {
            if (buf.tab[0].NumCategorie > mat)
                supB = *i - 1;
            if (mat > buf.tab[buf.nb_Enreg-1].NumCategorie)
                infB = *i + 1;
        }
    }
    if (infB>supB && mat<buf.tab[0].NumCategorie)
        *j = 0;
    if (infB>supB && mat>buf.tab[buf.nb_Enreg-1].NumCategorie) // le numéro recherché est plus grand que
    {
        if (buf.nb_Enreg == Max_Index) // le dernier bloc est plein
        {
            *i = infB; // il faut insérer l'enregistrement dans un nouveau bloc
            *j = 0;
        }
        else // le dernier bloc n'est pas encore plein
            *j = buf.nb_Enreg; // il faut l'insérer dans le dernier bloc à la première position libre
    }
}

/// ***************************************** INSERTION_index2 ***************************************** ///

int insertion_force_TOF (TOF_index* f, indexGategorieForce enreg)
{
    int i,j,trouv;
    rechDicho_force_TOF(enreg.NumCategorie,f,&trouv,&i,&j);
    BufferIndexForce buf;
    indexGategorieForce x;
    int k,continu=1;
    while ((continu==1) && (i<=entete2(f,1)))
    {
        lireDir2(f,i,&buf);
        x = buf.tab[buf.nb_Enreg-1]; // sauvegarder le dernier enregistrement du bloc
        /* Décalages intra-bloc */
        for(k = buf.nb_Enreg-1; k>j; k--)
            buf.tab[k] = buf.tab[k-1];
        buf.tab[j] = enreg;
        if(buf.nb_Enreg < Max_Index) // si le bloc n'est pas plein, on remet x dans permier indice libre du bloc
        {
            buf.nb_Enreg++;
            if (j == buf.nb_Enreg-1) // le nouvel enregistrement est le dernier enregistrement du fichier
                buf.tab[buf.nb_Enreg-2] = x;
            else // le nouvel enregistrement a été inséré entre deux enregistrements
                buf.tab[buf.nb_Enreg-1] = x;
            ecrireDir2(f,i,&buf);
            continu = 0;
        }
        else // si le bloc est plein, x doit être inséré dans un nouveau bloc i+1 à la position 0
        {
            ecrireDir2(f,i,&buf);
            i++;
            j = 0;
            enreg = x; // l'enregistrement qui doit être inséré à l'emplacement (i,0)
        }
    }
    // si on dépasse la fin de fichier, on rajoute un nouveau bloc contenant un seul enregistrement
    if (i>entete2(f,1))
    {
        buf.tab[0] = enreg;
        buf.nb_Enreg = 1;
        // alloc_bloc2(f);
        ecrireDir2(f,alloc_bloc2(f),&buf);
    }
    return 1;
}


/// ******************************** CREATION_index_categorie_FORCE *********************************** ///

TOF_index *creaIndexForce(TOF_index *f, LNOF *fichier, char nomfich[30])
{
    int i,j,nbEnreg,nbBloc,k,nb;
    int trouv2,bloc,depl;
    int numeroCategorie;
    Buffer buffer1;
    BufferIndexForce buf;
    indexGategorieForce enreg;
    f = Ouvrir("indexForce.bin",'N');
    nb = (fichier->entete).nbEnreg;
    k = 0;
    nbBloc = (fichier->entete).nbBloc; // pour recuperer les grades du fichier LNOF
    for (i=1; i<= nbBloc; i++)
    {
        lireDir(fichier,i,&buffer1);
        nb_l++;
        nbEnreg = buffer1.nb;
        for (j=0; j<nbEnreg; j++)
        {
            if (strcmp(buffer1.Tab[j].Force_Armee,"Armee_de_terre") == 0 ){
                enreg.NumCategorie = 1;
            }
            if (strcmp(buffer1.Tab[j].Force_Armee,"Armee_de_l_air") == 0 )            {
                enreg.NumCategorie = 2;
            }
            if (strcmp(buffer1.Tab[j].Force_Armee,"Marine_nationale") == 0 )            {
                enreg.NumCategorie = 3;
            }
            if (strcmp(buffer1.Tab[j].Force_Armee,"Defense_aerienne_du_territoire") == 0 )            {
                enreg.NumCategorie = 4;
            }
            if (strcmp(buffer1.Tab[j].Force_Armee,"Gendarmerie_nationale") == 0 )            {
                enreg.NumCategorie = 5;
            }
            if (strcmp(buffer1.Tab[j].Force_Armee,"Garde_republicaine") == 0 ){
                enreg.NumCategorie = 6;
            }
            if (strcmp(buffer1.Tab[j].Force_Armee,"Departement_du_renseignement_et_de_la_securite") == 0 ){
                enreg.NumCategorie = 7;
            }
            if (strcmp(buffer1.Tab[j].Force_Armee,"Sante_militaire") == 0 ){
                enreg.NumCategorie = 8;
            }
            enreg.i = i;
            enreg.j = j;
            enreg.effacement=0;
            insertion_force_TOF(f,enreg);
            gotoxy(10,10);
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tNombre d'enregistrement remplie : %d/%d",k+1,nb);
            printf("\n\n\t\t\t\t\tChargement ... %d \%",(k+1)*100/nb);
            gotoxy(10,10);
            k = k+1;
        }
    }
    return f;
}


/// ***************************************** AFFICHAGE_INDEX_Force ***************************************** ///


void Affichage3_TOFindex(TOF_index *F,char Nomfichier[30])
{
    int n,i,j;
    BufferIndexForce buffer;
    n = (F->entete).nb_bloc;
    printf(" \n          V O T R E      F I C H I E R         \n");
    for (i=1;i<=n;i++)
    {
        printf("\n\n ********** BLOC %d **********  :\n\n",i);
        lireDir2(F,i,&buffer);
        for (j=0;j<buffer.nb_Enreg;j++)
        {
            printf("\n Categorie : %d Bloc : %d  Deplacement : %d ",buffer.tab[j].NumCategorie,buffer.tab[j].i,buffer.tab[j].j+1);
        }
   }
}


/// ***************************************** AFFICHAGE_INDEX ***************************************** ///

void Affichage_TOFindex(TOF_index *F,char Nomfichier[30],int nb)
{
    int n,i,j;
    BufferIndex buffer;
    char bloc[10];
    char deplacement[20];
    char s[5];
    n = (F->entete).nb_bloc;
    for (i=1;i<=nb;i++)
    {
        textcolor(GREEN);
        printf("\n\t\t\t\t\t\t\t\t   B l o c   n u m e r o : %d\n",i);
        textcolor(LIGHTGREEN);
        printf("\n\t\t\t\t\t\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187,32,32,32,32,32);
        printf("\n\t\t\t\t\t\t%c   Matricule   %c  Bloc   %c   Deplacement   %c    Effacement    %c",186,186,186,186,186);
        printf("\n\t\t\t\t\t\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
        textcolor(WHITE);
        printf("\n\t\t\t\t\t\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187,32,32,32,32,32);
        lireDir2(F,i,&buffer);
        for (j=0;j<buffer.nb_Enreg;j++)
        {
            if (j== buffer.nb_Enreg -1)
            {
                s[0]=' ';
                itoa(buffer.tab[j].i,bloc,10);
                itoa(buffer.tab[j].j,deplacement,10);
                while (strlen(bloc)!=7)   strcat(bloc,s);
                while (strlen(deplacement)!=15)   strcat(deplacement,s);
                printf("\n\t\t\t\t\t\t%c     %d    %c %s %c %s %c %d                %c",186,buffer.tab[j].Matricule,186,bloc,186,deplacement,186,buffer.tab[j].effacement,186);
                printf("\n\t\t\t\t\t\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
            }
            else
            {
                s[0]=' ';
                itoa(buffer.tab[j].i,bloc,10);
                itoa(buffer.tab[j].j,deplacement,10);
                while (strlen(bloc)!=7)   strcat(bloc,s);
                while (strlen(deplacement)!=15)   strcat(deplacement,s);
                printf("\n\t\t\t\t\t\t%c     %d    %c %s %c %s %c %d                %c",186,buffer.tab[j].Matricule,186,bloc,186,deplacement,186,buffer.tab[j].effacement,186);
                printf("\n\t\t\t\t\t\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",204,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,206,205,205,205,205,205,205,205,205,205,206,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,206,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,185);
            }
        }
   }
   printf("\n\n\n\n");
}


/// ***************************************** AFFICHAGE_INDEX_Grade ***************************************** ///

void Affichage2_TOFindex(TOF_index *F,char Nomfichier[30])
{
    int n,i,j;
    BufferIndexGrade buffer;
    n = (F->entete).nb_bloc;
    printf(" \n          V O T R E      F I C H I E R         \n");
    for (i=1;i<=n;i++)
    {
        printf("\n\n ********** BLOC %d **********  :\n\n",i);
        lireDir2(F,i,&buffer);
        for (j=0;j<buffer.nb_Enreg;j++)
        {
            printf("\n Categorie : %d Bloc : %d  Deplacement : %d ",buffer.tab[j].NumCategorie,buffer.tab[j].i,buffer.tab[j].j+1);
        }
   }
}
/// ***************************************** RECHERCHE LNOF ***************************************** ///


void Suppression_CategoriesForce(char Nomfich[30], TOF_index*fichier,TOF_index*fich)
{
   LNOF *F;
   int trouv,i,j,numCategorie,bloc,depl,i_force,j_force,cpt=0;
   Buffer buf;
   BufferIndexForce buf1;
   F = Ouvrir(Nomfich,'A');
   textcolor(LIGHTMAGENTA);
   printf("\n\n\t ----------------------- LA SUPPRESSION DES CATEGORIES -----------------------\n\n");
   textcolor(WHITE);
   printf("\n \t1 : Armee de terre\n \t2 : Armee de l air\n \t3 : Marine nationale\n \t4 : Defense aerienne du territoire\n \t5 : Gendarmerie nationale\n \t6 : Garde republicaine\n \t7 : Departement du renseignement et de la securite\n \t8 : Sante militaire\n\n");
   printf("\nVeuillez saisir le numero correspond a la categorie que vous voulez supprimee : ");
   textcolor(LIGHTMAGENTA);
   scanf("%d",&numCategorie);
   textcolor(WHITE);
   rechDicho_force_TOF(numCategorie,fichier,&trouv,&i,&j);
   lireDir2(fichier,i,&buf1);
   bloc = buf1.tab[j].i;
   depl = buf1.tab[j].j;
   lireDir(F,bloc,&buf);
   supp_LNOF_2(F,fichier,bloc,depl);
   suppression_index(fich,buf.Tab[depl].Matricule);
   suppression_index_force(fichier,buf.Tab[depl].Matricule);
   if (trouv)
   {
        j--;
        while (buf1.tab[j].NumCategorie == numCategorie )
        {
           bloc = buf1.tab[j].i;
           depl = buf1.tab[j].j;
           lireDir(F,bloc,&buf);
           nb_l++;
           supp_LNOF_2(F,fichier,bloc,depl);
           //suppression_index(fich,buf.Tab[depl].Matricule);
           //suppression_index_force(fichier,buf.Tab[depl].Matricule);p
           j--;
           if(j == -1)
           {
               i--;
               lireDir2(fichier,i,&buf1);
               j = Max_Index - 1 ;
           }
        }
        rechDicho_force_TOF(numCategorie,fichier,&trouv,&i,&j);
        bloc = buf1.tab[j].i;
        depl = buf1.tab[j].j;
        lireDir2(fichier,i,&buf1);
        j++;
        while (buf1.tab[j].NumCategorie == numCategorie)
        {
           bloc = buf1.tab[j].i;
           depl = buf1.tab[j].j;
           lireDir(F,bloc,&buf);
           nb_l++;
           supp_LNOF_2(F,fichier,bloc,depl,&i_force,&j_force);//i_force et j_force pour mettre à jour l index force
           //suppression_index(fich,buf.Tab[depl].Matricule);
           //suppression_index_force(fichier,buf.Tab[depl].Matricule);
           j++;
           if(j == Max_Index )
           {
               i++;
               lireDir2(fichier,i,&buf1);
               j = 0;
           }
        }
   }
   textcolor(LIGHTGREEN);
   printf("\n\n\tla suppression a ete effectuee par succee");
   textcolor(WHITE);
}
void maj_dernier_enreg_force(LNOF *F,TOF_index *fichier,int nvi,int nvj,char rm[40],int oldi,int oldj)
{
    //printf("\nforce : %s,oldi=%d,oldj=%d",rm,oldi,oldj);
   BufferIndexForce buf1;
   int i,j,trouv,num;
   if (strcmp(rm,"Armee_de_terre") == 0 ){
            rechDicho_force_TOF(1,fichier,&trouv,&i,&j);
            num=1;
            }
            if (strcmp(rm,"Armee_de_l_air") == 0 )            {
              rechDicho_force_TOF(2,fichier,&trouv,&i,&j);
              num=2;
            }
            if (strcmp(rm,"Marine_nationale") == 0 )            {
                rechDicho_force_TOF(3,fichier,&trouv,&i,&j);
                num=3;
            }
            if (strcmp(rm,"Defense_aerienne_du_territoire") == 0 )            {
                rechDicho_force_TOF(4,fichier,&trouv,&i,&j);
                num=4;
            }
            if (strcmp(rm,"Gendarmerie_nationale") == 0 )            {
                 rechDicho_force_TOF(5,fichier,&trouv,&i,&j);
                 num=5;
            }
            if (strcmp(rm,"Garde_republicaine") == 0 ){
                 rechDicho_force_TOF(6,fichier,&trouv,&i,&j);
                 num=6;
            }
            if (strcmp(rm,"Departement_du_renseignement_et_de_la_securite") == 0 ){
                     rechDicho_force_TOF(7,fichier,&trouv,&i,&j);
                     num=7;
            }
            if (strcmp(rm,"Sante_militaire") == 0 ){
                 rechDicho_force_TOF(8,fichier,&trouv,&i,&j);
                 num=8;
            }
             lireDir2(fichier,i,&buf1);
             if((buf1.tab[j].i==oldi)&&(buf1.tab[j].j==oldj)){
               buf1.tab[j].i=nvi;
               buf1.tab[j].j=nvj;
               ecrireDir2(fichier,i,&buf1);
           }
   if (strcmp(rm,"Armee_de_terre") == 0 ){
            rechDicho_force_TOF(1,fichier,&trouv,&i,&j);
            num=1;
            }
            if (strcmp(rm,"Armee_de_l_air") == 0 )            {
              rechDicho_force_TOF(2,fichier,&trouv,&i,&j);
              num=2;
            }
            if (strcmp(rm,"Marine_nationale") == 0 )            {
                rechDicho_force_TOF(3,fichier,&trouv,&i,&j);
                num=3;
            }
            if (strcmp(rm,"Defense_aerienne_du_territoire") == 0 )            {
                rechDicho_force_TOF(4,fichier,&trouv,&i,&j);
                num=4;
            }
            if (strcmp(rm,"Gendarmerie_nationale") == 0 )            {
                 rechDicho_force_TOF(5,fichier,&trouv,&i,&j);
                 num=5;
            }
            if (strcmp(rm,"Garde_republicaine") == 0 ){
                 rechDicho_force_TOF(6,fichier,&trouv,&i,&j);
                 num=6;
            }
            if (strcmp(rm,"Departement_du_renseignement_et_de_la_securite") == 0 ){
                     rechDicho_force_TOF(7,fichier,&trouv,&i,&j);
                     num=7;
            }
            if (strcmp(rm,"Sante_militaire") == 0 ){
                 rechDicho_force_TOF(8,fichier,&trouv,&i,&j);
                 num=8;
            }
            lireDir2(fichier,i,&buf1);
        while (buf1.tab[j].NumCategorie ==num )
        {
           if((buf1.tab[j].i==oldi)&&(buf1.tab[j].j==oldj)){
               buf1.tab[j].i=nvi;
               buf1.tab[j].j=nvj;
               ecrireDir2(fichier,i,&buf1);
           }
           j--;
           if(j == -1)
           {
               i--;
               lireDir2(fichier,i,&buf1);
               j = Max_Index - 1 ;
           }
        }
        if (strcmp(rm,"Armee_de_terre") == 0 ){
            rechDicho_force_TOF(1,fichier,&trouv,&i,&j);
            num=1;
            }
            if (strcmp(rm,"Armee_de_l_air") == 0 )            {
              rechDicho_force_TOF(2,fichier,&trouv,&i,&j);
              num=2;
            }
            if (strcmp(rm,"Marine_nationale") == 0 )            {
                rechDicho_force_TOF(3,fichier,&trouv,&i,&j);
                num=3;
            }
            if (strcmp(rm,"Defense_aerienne_du_territoire") == 0 )            {
                rechDicho_force_TOF(4,fichier,&trouv,&i,&j);
                num=4;
            }
            if (strcmp(rm,"Gendarmerie_nationale") == 0 )            {
                 rechDicho_force_TOF(5,fichier,&trouv,&i,&j);
                 num=5;
            }
            if (strcmp(rm,"Garde_republicaine") == 0 ){
                 rechDicho_force_TOF(6,fichier,&trouv,&i,&j);
                 num=6;
            }
            if (strcmp(rm,"Departement_du_renseignement_et_de_la_securite") == 0 ){
                     rechDicho_force_TOF(7,fichier,&trouv,&i,&j);
                     num=7;
            }
            if (strcmp(rm,"Sante_militaire") == 0 ){
                 rechDicho_force_TOF(8,fichier,&trouv,&i,&j);
                 num=8;
            }
            lireDir2(fichier,i,&buf1);
            j++;
            while ( (buf1.tab[j].NumCategorie == num) && ( i <= (fichier->entete).nb_bloc ) ){
               if((buf1.tab[j].i==oldi)&&(buf1.tab[j].j==oldj)){
               buf1.tab[j].i=nvi;
               buf1.tab[j].j=nvj;
               ecrireDir2(fichier,i,&buf1);
           }
           j++;
           if(j == Max_Index )
           {
               i++;
               lireDir2(fichier,i,&buf1);
               j = 0;
           }
        }
    }
///*********************************************Index_AGE********************************************///
/// Les structures du fichier Index2 TOF ///

//---------------------------------------------//

typedef struct entete_index2
{
    int nb_bloc;

} entete_index2;

//---------------------------------------------//

typedef struct TOF_index2
{
    FILE *fichier;
    entete_index2 entete;

}TOF_index2;

//---------------------------------------------//

typedef struct index2
{
    int age;
    int i;
    int j;
    int effacement; // 1 si l'index est efface 0 sinon

}index2;

//---------------------------------------------//

typedef struct blocIndex2
{
    index2 tab[Max_Index];
    int nb_Enreg;

}blocIndex2;

//---------------------------------------------//

typedef struct blocIndex2 BufferIndex2;
int age1(Tenreg enrg)
{
    char *token;
    char year[10];
    token = strtok(enrg.Date_Naissance,"/");
    token = strtok(NULL, "/");
    token = strtok(NULL, "/");
    return 2022-atoi(token);
}
int age2(index2 enrg)
{
    return enrg.age;
}

void rechDicho_age_TOF(int agee, TOF_index *f, int* trouv,int* i,int* j)
{
    BufferIndex2 buf;
    *trouv = 0;
    int stop = 0;
    int infB, supB;
    int infE, supE;
    if (entete2(f,1) == 0) // en cas le fichier est vide
    {
        *i = 1;
        *j = 0;
        return;
    }
    /* Recherche externe */
    infB = 1; // le premier bloc
    supB = entete2(f, 1); //le dernier bloc
    while(infB<=supB && !(*trouv) && !stop)
    {
        *i = (infB + supB) / 2; // le numéro du bloc du milieu
        lireDir2(f, *i, &buf);
        if (age2(buf.tab[0]) <= agee && agee <= age2(buf.tab[buf.nb_Enreg-1]))
        {
            /* Recherche interne */
            infE = 0; // le premier enregistrement du bloc
            supE = buf.nb_Enreg - 1; // le dernier enregistrement du bloc
            while(infE<=supE && !(*trouv))
            {
                *j = (infE + supE) / 2; // l'indice de l'enregistrement du milieu
                if (age2(buf.tab[*j]) == agee)
                    *trouv = 1;
                else
                {
                    if (age2(buf.tab[*j])> agee)
                        supE = *j - 1;
                    if (age2(buf.tab[*j]) < agee)
                        infE = *j + 1;
                }
            }
            if (infE>supE) // le numéro recherché n'existe pas dans le bloc
                *j = infE; // la position ou il faut l'inséerer alors
            stop = 1;
            /* Fin de la recherche interne */
        }
        else
        {
            if (age2(buf.tab[0]) > agee)
                supB = *i - 1;
            if (agee > age2(buf.tab[buf.nb_Enreg-1]))
                infB = *i + 1;
        }
    }
    if (infB>supB && agee<age2(buf.tab[0]))
        *j = 0;
    if (infB>supB && agee>age2(buf.tab[buf.nb_Enreg-1])) // le numéro recherché est plus grand que
    {
        if (buf.nb_Enreg == Max_Index) // le dernier bloc est plein
        {
            *i = infB; // il faut insérer l'enregistrement dans un nouveau bloc
            *j = 0;
        }
        else // le dernier bloc n'est pas encore plein
            *j = buf.nb_Enreg; // il faut l'insérer dans le dernier bloc à la première position libre
    }
}
int insertion_age_TOF (TOF_index* f, index2 enreg)
{
    int i,j,trouv;
    rechDicho_age_TOF(enreg.age,f,&trouv,&i,&j);
    BufferIndex2 buf;
    index2 x;
    int k,continu=1;
    while ((continu==1) && (i<=entete2(f,1)))
    {
        lireDir2(f,i,&buf);
        x = buf.tab[buf.nb_Enreg-1]; // sauvegarder le dernier enregistrement du bloc
        /* Décalages intra-bloc */
        for(k = buf.nb_Enreg-1; k>j; k--)
            buf.tab[k] = buf.tab[k-1];
        buf.tab[j] = enreg;
        if(buf.nb_Enreg < Max_Index) // si le bloc n'est pas plein, on remet x dans permier indice libre du bloc
        {
            buf.nb_Enreg++;
            if (j == buf.nb_Enreg-1) // le nouvel enregistrement est le dernier enregistrement du fichier
                buf.tab[buf.nb_Enreg-2] = x;
            else // le nouvel enregistrement a été inséré entre deux enregistrements
                buf.tab[buf.nb_Enreg-1] = x;
            ecrireDir2(f,i,&buf);
            continu = 0;
        }
        else // si le bloc est plein, x doit être inséré dans un nouveau bloc i+1 à la position 0
        {
            ecrireDir2(f,i,&buf);
            i++;
            j = 0;
            enreg = x; // l'enregistrement qui doit être inséré à l'emplacement (i,0)
        }
    }
    // si on dépasse la fin de fichier, on rajoute un nouveau bloc contenant un seul enregistrement
    if (i>entete2(f,1))
    {
        buf.tab[0] = enreg;
        buf.nb_Enreg = 1;
        // alloc_bloc2(f);
        ecrireDir2(f,alloc_bloc2(f),&buf);
    }
    return 1;
}

TOF_index2 *creatiaonIndex_age(TOF_index *f, LNOF *fichier, char nomfich[30])   /** on va creer ce fichier a partir du fichier L/OVF mais trié par age**/
{
    int i,j,nbEnreg,nbBloc;
    int trouv2,bloc,depl;
    int age ,nb,k;
    Buffer buffer1;
    BufferIndex2 buf;
    index2 enreg;
    f = Ouvrir("index3.bin",'N');
    nb = (fichier->entete).nbEnreg;
    k = 0;
    nbBloc = (fichier->entete).nbBloc; // pour recuperer les matricules du fichier LNOF
    for (i=1; i<= nbBloc; i++)
    {
        lireDir(fichier,i,&buffer1);
        nb_l++;
        nbEnreg = buffer1.nb;
        for (j=0; j<nbEnreg; j++)
        {
            enreg.age = age1(buffer1.Tab[j]);
            enreg.i = i;
            enreg.j = j;
            enreg.effacement=0;
            insertion_age_TOF(f,enreg);
            gotoxy(10,10);
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tNombre d'enregistrement remplie : %d/%d",k+1,nb);
            printf("\n\n\t\t\t\t\tChargement ... %d \%",(k+1)*100/nb);
            gotoxy(10,10);
            k = k+1;
            lireDir2(f, i, &buf);
        }
    }
    return f;
}
void Affichage_TOFindex_age(TOF_index *F,char Nomfichier[30])
{
    int n,i,j;
    BufferIndex2 buffer;
    n = (F->entete).nb_bloc;
    printf(" \n          V O T R E      F I C H I E R         \n");
    for (i=1;i<=n;i++)
    {
        printf("\n\n ********** BLOC %d **********  :\n\n",i);
        lireDir2(F,i,&buffer);
        for (j=0;j<buffer.nb_Enreg;j++)
        {
            printf("\n  L'age : %d Bloc : %d  Deplacement : %d effacement logique : %d ",age2(buffer.tab[j]),buffer.tab[j].i,buffer.tab[j].j+1,buffer.tab[j].effacement);
        }
   }
}
void Consultation_age(char Nomfich[30], TOF_index*fichier)
{
   LNOF *F;
   int trouv,i,j,min,max,bloc,depl,sauv;
   char rm[40];
   Buffer buf;
   BufferIndex2 buf1;
   F = Ouvrir(Nomfich,'A');
   printf("\n\tdonnez la borne inf de l'age : ");
   textcolor(LIGHTGREEN);
   scanf("%d",&min);
   lireDir2(fichier,1,&buf1);
   if(min<buf1.tab[1].age){min=buf1.tab[1].age+1;}
   textcolor(WHITE);
   printf("\n\tdonnez la borne sup de l'age : ");
   textcolor(LIGHTGREEN);
   scanf("%d",&max);
   textcolor(WHITE);
   printf("\n\t\t\t\tveuillez choisir une de ces regions melitaire\n\t\t\t\t\t1) 1RM-Blida\n\t\t\t\t\t2) 2RM-Oran\n\t\t\t\t\t3) 3RM-Bechar\n\t\t\t\t\t4) 4RM-Ouargla\n\t\t\t\t\t5) 5RM-Constantine\n\t\t\t\t\t5) 6RM-Tamanrasset\n\n");
   printf("\n\tdonnez la region concerne : ");
   textcolor(LIGHTGREEN);
   scanf("%s",&rm);
   textcolor(WHITE);
   rechDicho_age_TOF(min,fichier,&trouv,&i,&j);
   lireDir2(fichier,i,&buf1);
   bloc = buf1.tab[j].i;
   depl = buf1.tab[j].j;
   printf("\nles personnes dont l'age est compris entre %d et %d et relatifs a la region melitaire %s sont : \n",min,max,rm);
   lireDir(F,bloc,&buf);
   nb_l++;
   if((bloc<4000)&&(strcmp(rm,buf.Tab[depl].Region_Militaire)==  0)&&(age1(buf.Tab[depl])<=max)&&(age1(buf.Tab[depl])>=min)){
   printf("\n");
   textcolor(LIGHTGREEN );
   printf("              ----------------------------------------------------------------------------------------------------------");
   textcolor(WHITE);
   printf("\n\t\t  MATRICULE : ");
   textcolor(GREEN);
   printf("%d",buf.Tab[depl].Matricule);
   textcolor(WHITE);
   printf("  NOM  : ");
   textcolor(GREEN);
   printf("%s",buf.Tab[depl].Nom);
   textcolor(WHITE);
   printf("  region  : ");
   textcolor(GREEN);
   printf("%s",buf.Tab[depl].Region_Militaire);
   textcolor(WHITE);
   printf(" AGE : ");
   textcolor(GREEN);
   printf("%d\n",age1(buf.Tab[depl]));
   textcolor(LIGHTGREEN);
   printf("              ----------------------------------------------------------------------------------------------------------");
   textcolor(WHITE);}
   if (age1(buf.Tab[depl])<=max)
   {
        j--;
        while (buf1.tab[j].age<=max )
        {
           bloc = buf1.tab[j].i;
           depl = buf1.tab[j].j;
           lireDir(F,bloc,&buf);
           nb_l++;
              if((bloc<4000)&&(strcmp(rm,buf.Tab[depl].Region_Militaire)==  0)&&(age1(buf.Tab[depl])<=max)&&(age1(buf.Tab[depl])>=min)){
                    printf("\n");
   textcolor(LIGHTGREEN );
   printf("              ----------------------------------------------------------------------------------------------------------");
   textcolor(WHITE);
   printf("\n\t\t  MATRICULE : ");
   textcolor(GREEN);
   printf("%d",buf.Tab[depl].Matricule);
   textcolor(WHITE);
   printf("  NOM  : ");
   textcolor(GREEN);
   printf("%s",buf.Tab[depl].Nom);
   textcolor(WHITE);
   printf("  region  : ");
   textcolor(GREEN);
   printf("%s",buf.Tab[depl].Region_Militaire);
   textcolor(WHITE);
   printf(" AGE : ");
   textcolor(GREEN);
   printf("%d\n",age1(buf.Tab[depl]));
   textcolor(LIGHTGREEN);
   printf("              ----------------------------------------------------------------------------------------------------------");
   textcolor(WHITE);}
           j--;
           if(j == -1)
           {
               i--;
               lireDir2(fichier,i,&buf1);
               j = Max_Index - 1 ;
           }
        }
         rechDicho_age_TOF(min,fichier,&trouv,&i,&j);
        lireDir2(fichier,i,&buf1);
        j++;
        while ( (buf1.tab[j].age<=max) && ( i <= (fichier->entete).nb_bloc ) )
        {
           bloc = buf1.tab[j].i;
           depl = buf1.tab[j].j;
           if((bloc<4000)&&(strcmp(rm,buf.Tab[depl].Region_Militaire)==  0)&&(age1(buf.Tab[depl])<=max)&&(age1(buf.Tab[depl])>=min)){
              printf("\n");
   textcolor(LIGHTGREEN );
   printf("              ----------------------------------------------------------------------------------------------------------");
   textcolor(WHITE);
   printf("\n\t\t  MATRICULE : ");
   textcolor(GREEN);
   printf("%d",buf.Tab[depl].Matricule);
   textcolor(WHITE);
   printf("  NOM  : ");
   textcolor(GREEN);
   printf("%s",buf.Tab[depl].Nom);
   textcolor(WHITE);
   printf("  region  : ");
   textcolor(GREEN);
   printf("%s",buf.Tab[depl].Region_Militaire);
   textcolor(WHITE);
   printf(" AGE : ");
   textcolor(GREEN);
   printf("%d\n",age1(buf.Tab[depl]));
   textcolor(LIGHTGREEN);
   printf("              ----------------------------------------------------------------------------------------------------------");
   textcolor(WHITE);}
           j++;
           if(j == Max_Index )
           {
               i++;
               lireDir2(fichier,i,&buf1);
               j = 0;
           }
        }
   }
   printf("\n\n");
   Fermer(F);
}


/// ********************************** Les fonctions et procedures de fichier L/OF ********************************** ///

/// ***************************************** GENERATION DES NOMS/PRENOMS ***************************************** ///

int generer_bornes(int min, int max) // Génerer un nombre entre min et max
{
    return rand()%(max-min+1) + min;
}
//-----------------------------------------------------///
char Recuperer_lettre(char tableau[30],int numero) // Recupérer la lettre qui correspond au numero de lettre
{
    return tableau[numero-1];
}
//-----------------------------------------------------///
void GenererNom(char Nom[40])
{
    char tableau[40];
    int a, b, k, j, i = 97;
    for (j=0; j<26; ++j)
    {
        tableau[j] = i;    // Tableau d'Alphabet
        i++;
    }
    a = generer_bornes(4,13);
    for(k=0; k<a; ++k)
    {
        b = generer_bornes(1,26);
        Nom[k] = Recuperer_lettre(tableau,b);
    }
    Nom[a] = '\0'; // Fin de la chaine
}

/// ***************************************** GENERATION DES WILAYAS ***************************************** ///

void GenererWilaya(char wilaya[30])
{
    int i;
    i = generer_bornes(0,57);
    strcpy(wilaya,Wilayas[i]);
}

/// ***************************************** GENERATION DE GRADE ***************************************** ///

void GenererGrade(char grade[30])
{
    int i;
    i= generer_bornes(0,16);
    strcpy(grade,Grade[i]);
}

/// ***************************************** GENERATION DE REGION MILITAIRE ***************************************** ///

void GenererRM (char RM[30])
{
    int i;
    i= generer_bornes(0,5);
    strcpy(RM,RegionMilitaire[i]);
}

/// ***************************************** GENERATION DE FORCE ARMEE ***************************************** ///

void GenererFM (char FM[30])
{
    int i;
    i= generer_bornes(0,5);
    strcpy(FM,force_armee[i]);
}

/********************************************* GENERATION DE L ANNEE DE NAISSANCE ****************************/

int RandomNumberGenerator(const int nMin, const int nMax)
{
  return rand()%(nMax-nMin) + nMin;
}

//-----------------------------------------------------///

void turn_to_string(char chaine[], int n, int longueur)
{
    int k;
    for(k=longueur-1;k>=0;k--)
    {
     chaine[k]=(n%10)+'0';
     n=n/10;
    }
    chaine[longueur]='\0';
}

//-----------------------------------------------------///

char* generer_date_de_naissance(char chaine[])
{
    int mois,jour,annee;
    annee=RandomNumberGenerator(1940,2003);
    mois=RandomNumberGenerator(1,12);
    if((mois==1)||(mois==3)||(mois==5)||(mois==7)||(mois==8)||(mois==10)||(mois==12))
    {
        jour=RandomNumberGenerator(1,31);
    }
    else if(mois==2)
    {
        jour=RandomNumberGenerator(1,28);
    }
    else
    {
        jour=RandomNumberGenerator(1,30);
    }
    char an[10];
    char mo[10];
    char jo[10];
    chaine[0]='\0';
    turn_to_string(an,annee,4);
    turn_to_string(mo,mois,2);
    turn_to_string(jo,jour,2);
    strcat(chaine,jo);
    strcat(chaine,"/");
    strcat(chaine,mo);
    strcat(chaine,"/");
    strcat(chaine,an);
    chaine[10]='\0';
    return chaine;

}

/// ***************************************** GENERATION DE MATRICULE ***************************************** ///

long GenererMatricule()
{
    int i;
    i = generer_bornes(1,5);
    switch(i)
    {
        case 1:
            return generer_bornes(111111,200000);
            break;
        case 2:
            return generer_bornes(200000,300000);
            break;
        case 3:
            return generer_bornes(300000,400000);
            break;
        case 4:
            return generer_bornes(500000,700000);
            break;
        case 5:
            return generer_bornes(700000,999999);
            break;
    }
}
/// ***************************************** GENERATION DE GROUPE SANGUIN ***************************************** ///

void GenererGS (char GS[30])
{
    int i;
    i= generer_bornes(0,5);
    strcpy(GS,groupe_sanguin[i]);
}

/// ***************************************** CHARGEMENT INITIAL ***************************************** ///


LNOF *ChargementIni(LNOF *F,char Nomfichier[30],int nbPersonne)
{
    TBloc buffer;
    int i,j,k,nb,n,ne;
    int B;
    F = Ouvrir(Nomfichier,'N');
    j = 0; k = 1;
    B = (50*Max)/100;
    for (i=0;i<nbPersonne;i++)
    {
         alloc_bloc(F);
         if (j < B)
         {
             GenererNom(buffer.Tab[j].Nom);
             GenererNom(buffer.Tab[j].Prenom);
             GenererGrade(buffer.Tab[j].Grade);
             generer_date_de_naissance(buffer.Tab[j].Date_Naissance);
             GenererRM(buffer.Tab[j].Region_Militaire);
             GenererWilaya(buffer.Tab[j].Wilaya_Naissance);
             GenererGS(buffer.Tab[j].Groupe_Sanguin);
             GenererFM(buffer.Tab[j].Force_Armee);
             buffer.Tab[j].Matricule = GenererMatricule();
             j++;
         }
         else
         {
             buffer.nb = j;
             buffer.suivant = k+1;
             ecrireDir(F,k,&buffer);nb_e++;
             GenererNom(buffer.Tab[0].Nom);
             GenererNom(buffer.Tab[0].Prenom);
             GenererGrade(buffer.Tab[0].Grade);
             generer_date_de_naissance(buffer.Tab[0].Date_Naissance);
             GenererRM(buffer.Tab[0].Region_Militaire);
             GenererWilaya(buffer.Tab[0].Wilaya_Naissance);
             buffer.Tab[0].Matricule = GenererMatricule();
             GenererGS(buffer.Tab[0].Groupe_Sanguin);
             GenererFM(buffer.Tab[0].Force_Armee);
             j=1;k++;
         }
         gotoxy(10,10);
         printf("\n\n\n\t\t\t\t\t\t\t\tNombre d'enregistrement remplie : %d/%d",i+1,nbPersonne);
         printf("\n\n\t\t\t\t\t\t\t\tChargement ... %d \%",(i+1)*100/nbPersonne);
         gotoxy(10,10);
    }
    buffer.nb = j;
    buffer.suivant = -1;
    ecrireDir(F,k,&buffer);nb_e++;
    aff_entete(F,1,k);
    aff_entete(F,2,1);
    aff_entete(F,3,k);
    aff_entete(F,4,nbPersonne);
    textcolor(LIGHTBLUE);
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t Votre fichier a ete creer avec succes !");
    textcolor(WHITE);
    return F;
}



void gotoxy(int x, int y)
{
  COORD Pos = { x - 1, y - 1};
  SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

/// ***************************************** INSERTION ***************************************** ///


void Insertion(TOF_index *fichier,char Nomfichier[30])
{
    int trouv,bloc,depl;
    LNOF *F;
    index enreg;
    char nom[40],prenom[40],grade[40],RG[40],wilaya[40],dateNaissance[40],gs[40],fa[40];
    long matricule;
    Buffer buffer;
    int n,j,nbEnregi,blocs,q,a;
    F = Ouvrir(Nomfichier,'A');
    printf("\n\n\n\t\tVeuillez saisir les informations suivantes : \n");
    printf("\n\t\t\t ==> Le matricule : ");
    scanf("%d",&matricule);
    Recherche_LNOF(F,fichier,matricule,&trouv,&bloc,&depl);
    if (trouv)
    {
        printf("\n\n\n\t Ce matricule existe deja ! ");
    }
    else
    {
        printf("\n\t\t\t ==> Le nom : ");
        scanf("%s",nom);
        printf("\n\t\t\t ==> Le prenom : ");
        scanf("%s",prenom);
        printf("\n\t\t\t ==> La date de naissance : ");
        scanf("%s",dateNaissance);
        printf("\n\t\t\t ==> La wilaya de naissance : ");
        scanf("%s",wilaya);
        printf("\n\t\t\t ==> Le groupe sanguin : ");
        scanf("%s",gs);
        printf("\n\t\t\t ==> La force armee : ");
        scanf("%s",fa);
        printf("\n\t\t\t ==> Le grade : ");
        scanf("%s",grade);
        printf("\n\t\t\t ==> La region militaire : ");
        scanf("%s",RG);
        n = (F->entete).queue;
        lireDir(F,n,&buffer);nb_l++;
        nb_l++;
        if (buffer.nb == Max)
        {
            alloc_bloc(F);
            buffer.nb = 1;
            buffer.suivant = -1;
            strcpy(buffer.Tab[0].Nom,nom);
            strcpy(buffer.Tab[0].Prenom,prenom);
            strcpy(buffer.Tab[0].Grade,grade);
            strcpy(buffer.Tab[0].Region_Militaire,RG);
            strcpy(buffer.Tab[0].Wilaya_Naissance,wilaya);
            strcpy(buffer.Tab[0].Date_Naissance,dateNaissance);
            strcpy(buffer.Tab[0].Groupe_Sanguin,gs);
            strcpy(buffer.Tab[0].Force_Armee,fa);
            buffer.Tab[0].Matricule = matricule;

            q = (F->entete).queue + 1;
            ecrireDir(F,q,&buffer);nb_e++;
            blocs = (F->entete).nbBloc;
            aff_entete(F,1,blocs);
            aff_entete(F,3,q);
            a = n-1 ;
            lireDir(F,a,&buffer);nb_l++;
            nb_l++;
            buffer.suivant = q;
            ecrireDir(F,n,&buffer);nb_e++;
            enreg.Matricule = matricule;
            enreg.i = a;
            enreg.j = 0;
            enreg.effacement = 0;
            insertion_TOF(fichier,enreg);
        }
        else
        {
            strcpy(buffer.Tab[buffer.nb].Nom,nom);
            strcpy(buffer.Tab[buffer.nb].Prenom,prenom);
            strcpy(buffer.Tab[buffer.nb].Grade,grade);
            strcpy(buffer.Tab[buffer.nb].Region_Militaire,RG);
            strcpy(buffer.Tab[buffer.nb].Wilaya_Naissance,wilaya);
            strcpy(buffer.Tab[buffer.nb].Date_Naissance,dateNaissance);
            strcpy(buffer.Tab[buffer.nb].Groupe_Sanguin,gs);
            strcpy(buffer.Tab[buffer.nb].Force_Armee,fa);
            buffer.Tab[buffer.nb].Matricule = matricule;
            buffer.nb ++;
            ecrireDir(F,n,&buffer);nb_e++;
            enreg.Matricule = matricule;
            enreg.i = n;
            enreg.j = buffer.nb;
            enreg.effacement = 0;
            insertion_TOF(fichier,enreg);
        }
        nbEnregi = (F->entete).nbEnreg +1;
        aff_entete(F,4,nbEnregi);
        printf("\n\n\n\t L'enregistrement a ete creer avec succes ! ");
    }
   Fermer(F);
}
/// ***************************************** SUPPRESSION DES DOUBLONS ***************************************** ///
void supp_LNOF_doublons(LNOF *F,int i,int j)//la supression doit etre physique
{
   int trouv,N;
   Buffer buf1;
   Buffer buf2,buf3;
   if(trouv==0)
   {
       printf("cet enregistrement n'existe pas dans le fichier");
   }
   else
   {
      N =entete( F, 1 );
      if(i!=N){
        lireDir(F,i,&buf1);nb_l++;
        nb_l++;nb_l++;
        lireDir( F,N,&buf2 );nb_l++;
        buf1.Tab[j]=buf2.Tab[buf2.nb-1];
        buf2.Tab[buf2.nb]=buf2.Tab[buf2.nb-1];
        buf2.nb=buf2.nb-1;
        ecrireDir(F,i,&buf1);nb_e++;
        if(buf2.nb>0){
           aff_entete(F,4,entete(F,4)-1);
            ecrireDir(F,N,&buf2);nb_e++;
        }
        else{
            aff_entete(F,1,N-1);
            aff_entete(F,2,1);
            aff_entete(F,3,N-1);
            aff_entete(F,4,entete(F,4)-1);
            lireDir(F,N-1,&buf3);
            nb_l++;
            buf3.suivant=-1;
            ecrireDir(F,N-1,&buf3);nb_e++;
        }
      }
      else
      {
          lireDir( F, N, &buf1 );nb_l++;
          buf1.Tab[j]=buf1.Tab[buf1.nb-1];
          buf1.Tab[buf2.nb]=buf1.Tab[buf2.nb-1];
          buf1.nb=buf1.nb-1;
          if(buf1.nb>0){
          aff_entete(F,4,entete(F,4)-1);
          ecrireDir(F,N,&buf1);nb_e++;
          }
          else{
            aff_entete(F,1,N-1);
            aff_entete(F,2,1);
            aff_entete(F,3,N-1);
            aff_entete(F,4,entete(F,4)-1);
            lireDir(F,N-1,&buf2);nb_l++;
            buf2.suivant=-1;
            ecrireDir(F,N-1,&buf2);nb_e++;
        }
   }
}
}
void supprimer_doublons(LNOF *F,TOF_index *fichier)
{
    Buffer buffer;
    BufferIndex buf;
    int j,bloc,depl,i,trouv;
    textcolor(YELLOW);
    printf("\n\n\n\t\t\t\tveuillez patientez jusqu'a ce que l'epuration soit fini :\n");
    textcolor(WHITE);
    for (i=1;i<=entete(F,1);i++)
    {
        j=0;
        lireDir(F,i,&buffer);nb_l++;
        while(j<buffer.nb)
        {
            rechDicho_TOF(buffer.Tab[j].Matricule,fichier,&trouv,&bloc,&depl);
            lireDir2(fichier,bloc,&buf);nb_l++;
            if((j!=buf.tab[depl].j)||(i!=buf.tab[depl].i))
               {
                   supp_LNOF_doublons(F,i,j);
               }

            j++;
        }
        gotoxy(10,10);
        printf("\n\n\t\t\t\t\t\t\t\tChargement ... %d",(i)*100/entete(F,1));
    }
    textcolor(LIGHTBLUE);
    printf("\n\n\t\t\t\t\t\t les doublons ont ete supprimes avec succes avec succes !");
    textcolor(WHITE);
}

/*
void Insertion(TOF_index *fichier,char Nomfichier[30])
{
    LNOF *F;
    int trouv,bloc,depl;
    index enreg;
    char nom[40],prenom[40],grade[40],RG[40],wilaya[40],dateNaissance[40],gs[40],fa[40];
    long matricule;
    Buffer buffer;
    int n,j,nbEnregi,blocs,q,a;
    F = Ouvrir(Nomfichier,'A');
    printf("Veuillez saisir les informations suivantes : \n");
    printf("\n le matricule : ");
    scanf("%d",&matricule);
    printf("\n le nom : ");
    scanf("%s",nom);
    printf("\n le prenom : ");
    scanf("%s",prenom);
    printf("\n la date de naissance : ");
    scanf("%s",dateNaissance);
    printf("\n la wilaya de naissance : ");
    scanf("%s",wilaya);
    printf("\n le groupe sanguin : ");
    scanf("%s",gs);
    printf("\n la force armee : ");
    scanf("%s",fa);
    printf("\n le grade : ");
    scanf("%s",grade);
    printf("\n la region militaire : ");
    scanf("%s",RG);
    Recherche_LNOF(F,fichier,matricule,&trouv,&bloc,&depl);
    if (trouv)
    {
        printf(" Ce matricule existe deja ! veuillez saisir un autre matricule ");
    }
    else
    {
        n = (F->entete).queue;
        lireDir(F,n,&buffer);
        if (buffer.nb == Max)
        {
            alloc_bloc(F);
            buffer.nb = 1;
            buffer.suivant = -1;
            strcpy(buffer.Tab[0].Nom,nom);
            strcpy(buffer.Tab[0].Prenom,prenom);
            strcpy(buffer.Tab[0].Grade,grade);
            strcpy(buffer.Tab[0].Region_Militaire,RG);
            strcpy(buffer.Tab[0].Wilaya_Naissance,wilaya);
            strcpy(buffer.Tab[0].Date_Naissance,dateNaissance);
            strcpy(buffer.Tab[0].Groupe_Sanguin,gs);
            strcpy(buffer.Tab[0].Force_Armee,fa);
            buffer.Tab[0].Matricule = matricule;

            q = (F->entete).queue + 1;
            ecrireDir(F,q,&buffer);
            blocs = (F->entete).nbBloc;
            aff_entete(F,1,blocs);
            aff_entete(F,3,q);
            a = n-1 ;
            lireDir(F,a,&buffer);
            buffer.suivant = q;
            ecrireDir(F,n,&buffer);
            enreg.Matricule = matricule;
            enreg.i = a;
            enreg.j = 0;
            insertion_TOF(fichier,enreg);
        }
        else
        {
            strcpy(buffer.Tab[buffer.nb].Nom,nom);
            strcpy(buffer.Tab[buffer.nb].Prenom,prenom);
            strcpy(buffer.Tab[buffer.nb].Grade,grade);
            strcpy(buffer.Tab[buffer.nb].Region_Militaire,RG);
            strcpy(buffer.Tab[buffer.nb].Wilaya_Naissance,wilaya);
            strcpy(buffer.Tab[buffer.nb].Date_Naissance,dateNaissance);
            strcpy(buffer.Tab[buffer.nb].Groupe_Sanguin,gs);
            strcpy(buffer.Tab[buffer.nb].Force_Armee,fa);
            buffer.Tab[buffer.nb].Matricule = matricule;
            buffer.nb ++;
            ecrireDir(F,n,&buffer);
            enreg.Matricule = matricule;
            enreg.i = n;
            enreg.j = buffer.nb;
            insertion_TOF(fichier,enreg);
        }
        nbEnregi = (F->entete).nbEnreg +1;
        aff_entete(F,4,nbEnregi);

    }
    Fermer(F);
}
*/
/// ***************************************** AFFICHAGE LNOF ***************************************** ///

void Affichage_LNOF(LNOF *F,char Nomfichier[30])
{
    int nb,n,i,j,ne;
    Buffer buffer;
    F = Ouvrir(Nomfichier,'A');
    n = (F->entete).nbBloc;
    i = 1;
    lireDir(F,n,&buffer);nb_l++;
    /**
    printf("\n\n\t  -------------------------------------------- Affichage du dernier bloc -------------------------------------------- \n\n");
    for (j=0;j<buffer.nb;j++)
    {
            printf("\n======> L'enregistrement numero '%d' \n",j+1);
            printf("\n  Matricule : %d\n  Nom : %s\n  Prenom : %s",buffer.Tab[j].Matricule,buffer.Tab[j].Nom,buffer.Tab[j].Prenom);
            printf("\n  Wilaya de naissance : %s",buffer.Tab[j].Wilaya_Naissance);
            printf("\n  Le groupe sanguin : %s\n  Le grade : %s\n  Region militaire : %s\n  Force armee : %s\n  ",buffer.Tab[j].Groupe_Sanguin,buffer.Tab[j].Grade,buffer.Tab[j].Region_Militaire,buffer.Tab[j].Force_Armee);
    }
    **/
}


void Affichage_DernierBloc(LNOF *F,char Nomfichier[30])
{
    int nb,n,i,j,ne;
    Buffer buffer;
    F = Ouvrir(Nomfichier,'A');
    n = (F->entete).nbBloc;
    i = 1;
    lireDir(F,n,&buffer);nb_l++;
    textcolor(YELLOW);
    printf("\n\n\t  -------------------------------------------- Affichage du dernier bloc -------------------------------------------- \n\n");
    textcolor(WHITE);
    for (j=0;j<buffer.nb;j++)
    {
            textcolor(LIGHTGREEN);
            printf("\n   ======> L'enregistrement numero '%d' \n",j+1);
            textcolor(WHITE);
            printf("\n  Matricule : %d\n  Nom : %s\n  Prenom : %s",buffer.Tab[j].Matricule,buffer.Tab[j].Nom,buffer.Tab[j].Prenom);
            printf("\n  Wilaya de naissance : %s",buffer.Tab[j].Wilaya_Naissance);
            printf("\n  Le groupe sanguin : %s\n  Le grade : %s\n  Region militaire : %s\n  Force armee : %s\n  ",buffer.Tab[j].Groupe_Sanguin,buffer.Tab[j].Grade,buffer.Tab[j].Region_Militaire,buffer.Tab[j].Force_Armee);
    }
}

/// ***************************************** RECHERCHE LNOF ***************************************** ///

void Recherche_LNOF(LNOF *F, TOF_index *fichier, long matricule, int *trouv2, int *bloc, int *depl)
{
    int trouv,i,j;
    Buffer buf;
//    LNOF *F;
    BufferIndex buf1;
  //  ouvrir2(fichier,"index.bin",'A');
    rechDicho_TOF(matricule,fichier,&trouv,&i,&j);
    *trouv2 = trouv;
    if (trouv)
    {
        lireDir2(fichier,i,&buf1);
        *bloc = buf1.tab[j].i;
        *depl = buf1.tab[j].j;
        lireDir(F,*bloc,&buf);
      //  printf("\n Ce matricule se trouve dans le bloc : %d\n l'enregistrement numero : %d",*bloc,*depl+1);
      //  printf("\n Le nom prenom associee a ce matricule est : %s\n Le prenom : %s\n La wilaya : %s\n ",buf.Tab[*depl].Nom, buf.Tab[*depl].Prenom, buf.Tab[*depl].Wilaya_Naissance);
    }
    else
    {
        trouv = 0;
    }
   // fermer2(fichier);
}



void Consultation_CategoriesGrades(char Nomfich[30], TOF_index*fichier)
{
   LNOF *F;
   int trouv,i,j,numCategorie,bloc,depl;
   Buffer buf;
   BufferIndexGrade buf1;
   F = Ouvrir(Nomfich,'A');
   printf("\n\n\t\t\t\t ----------------------- LA CONSULTAION DES CATEGORIES -----------------------\n\n");
   printf("\n\t\t\t\t\t1) Officiers-generaux\n\t\t\t\t\t2) Officiers-superieurs\n\t\t\t\t\t3) Officiers\n\t\t\t\t\t4) Sous-officiers\n\t\t\t\t\t5) Hommes de troupes\n\n");
   printf("\n\tVeuillez saisir le numero correspond aux categorie que vous voulez afficher : ");
   scanf("%d",&numCategorie);
   printf("\n");
   rechDicho2_TOF(numCategorie,fichier,&trouv,&i,&j);
   lireDir2(fichier,i,&buf1);
   bloc = buf1.tab[j].i;
   depl = buf1.tab[j].j;
   lireDir(F,bloc,&buf);
   textcolor(LIGHTGREEN);
   printf("              ----------------------------------------------------------------------------------------------------------");
   textcolor(WHITE);
   printf("\n\t\t  MATRICULE : ");
   textcolor(GREEN);
   printf("%d",buf.Tab[depl].Matricule);
   textcolor(WHITE);
   printf("  NOM  : ");
   textcolor(GREEN);
   printf("%s",buf.Tab[depl].Nom);
   textcolor(WHITE);
   printf("  PRENOM  : ");
   textcolor(GREEN);
   printf("%s",buf.Tab[depl].Prenom);
   textcolor(WHITE);
   printf(" GRADE : ");
   textcolor(GREEN);
   printf("%s\n",buf.Tab[depl].Grade);
   textcolor(LIGHTGREEN);
   printf("              ----------------------------------------------------------------------------------------------------------");
   textcolor(WHITE);
   if (trouv)
   {
       // printf("\n %d  %d   %d",trouv,i,j);
        j--;
        while (buf1.tab[j].NumCategorie == numCategorie )
        {
           bloc = buf1.tab[j].i;
           depl = buf1.tab[j].j;
           lireDir(F,bloc,&buf);nb_l++;
           printf("\n\t\t  MATRICULE : ");
           textcolor(GREEN);
           printf("%d",buf.Tab[depl].Matricule);
           textcolor(WHITE);
           printf("  NOM  : ");
           textcolor(GREEN);
           printf("%s",buf.Tab[depl].Nom);
           textcolor(WHITE);
           printf("  PRENOM  : ");
           textcolor(GREEN);
           printf("%s",buf.Tab[depl].Prenom);
           textcolor(WHITE);
           printf(" GRADE : ");
           textcolor(GREEN);
           printf("%s\n",buf.Tab[depl].Grade);
           textcolor(LIGHTGREEN);
   printf("              ----------------------------------------------------------------------------------------------------------");
           textcolor(WHITE);
           j--;
           if(j == -1)
           {
               i--;
               lireDir2(fichier,i,&buf1);
               j = Max_Index - 1 ;
           }
        }
        rechDicho2_TOF(numCategorie,fichier,&trouv,&i,&j);
        lireDir2(fichier,i,&buf1);
        j++;
        while ( (buf1.tab[j].NumCategorie == numCategorie) && ( i <= (fichier->entete).nb_bloc ) )
        {
           bloc = buf1.tab[j].i;
           depl = buf1.tab[j].j;
           lireDir(F,bloc,&buf);nb_l++;
                   //   printf("\n hahahaha222 %d   %d \n",bloc,depl);
           printf("\n\t\t  MATRICULE : ");
           textcolor(GREEN);
           printf("%d",buf.Tab[depl].Matricule);
           textcolor(WHITE);
           printf("  NOM  : ");
           textcolor(GREEN);
           printf("%s",buf.Tab[depl].Nom);
           textcolor(WHITE);
           printf("  PRENOM  : ");
           textcolor(GREEN);
           printf("%s",buf.Tab[depl].Prenom);
           textcolor(WHITE);
           printf(" GRADE : ");
           textcolor(GREEN);
           printf("%s\n",buf.Tab[depl].Grade);
           textcolor(LIGHTGREEN);
   printf("              ----------------------------------------------------------------------------------------------------------");
           textcolor(WHITE);
           j++;
           if(j == Max_Index )
           {
               i++;
               lireDir2(fichier,i,&buf1);
               j = 0;
           }
        }
   }
   printf("\n\n");
   Fermer(F);
}


/// ***************************************** RECHERCHE LNOF ***************************************** ///




LNOF *Affichage_bloc(char Nomfich[30],int nb)
{
    Buffer buf;
    LNOF *f;
    int i,j,cpt,k;
    long mat;
    char nom[40];
    char prenom[40];
    char date[60];
    char wilaya[60];
    char gs[60];
    char grade[60];
    char fa[60];
    char Rm[60];

    char s[3];
  //  f = Ouvrir("PERSONNEL-ANP_DZ.bin",'A');
    f = Ouvrir(Nomfich,'A');

    s[0]=' ';
        for (i=1;i<=nb;i++)
        {
            textcolor(LIGHTMAGENTA);
           ; printf("\n\n\n\t                  ***************************************************");
            textcolor(WHITE);
            printf(" BLOC NUMERO %d  ",i);
            textcolor(LIGHTMAGENTA);
            printf("***************************************************\n\n");
            lireDir(f,i,&buf);nb_l++;
            textcolor(LIGHTRED);
            printf("\n\t\t Ce bloc contient :");
            textcolor(WHITE);
            printf("\n\t\t         - %d enregistrements ",buf.nb);
            printf("\n\t\t         - Le numero du prochain bloc est : %d",buf.suivant);
            printf("\n\n\n");
            textcolor(YELLOW);
            printf("\n%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187,32,32,32,32,32);
            printf("\n%c Matricule %c     Nom      %c    Prenom    %c Datenaiss  %c      Wilaya     %c GS %c          Grade          %c             Force armeee             %c Region militaire   %c",186,186,186,186,186,186,186,186,186,186);
            printf("\n%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
            textcolor(WHITE);
            printf("\n%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,203,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187,32,32,32,32,32);
            for(j=0;j<buf.nb;j++)
            {
               if (j == buf.nb-1)
                {
                    strcpy(nom,buf.Tab[j].Nom);
                    strcpy(prenom,buf.Tab[j].Prenom);
                    strcpy(date,buf.Tab[j].Date_Naissance);
                    strcpy(wilaya,buf.Tab[j].Wilaya_Naissance);
                    strcpy(gs,buf.Tab[j].Groupe_Sanguin);
                    strcpy(grade,buf.Tab[j].Grade);
                    strcpy(fa,buf.Tab[j].Force_Armee);
                    strcpy(Rm,buf.Tab[j].Region_Militaire);
                    while (strlen(nom)!=13)   strcat(nom,s);
                    while (strlen(prenom)!=13)   strcat(prenom,s);
                    while (strlen(wilaya)!=17)   strcat(wilaya,s);
                    while (strlen(gs)!=4)   strcat(gs,s);
                    while (strlen(grade)!=25)   strcat(grade,s);
                    while (strlen(fa)!=37)   strcat(fa,s);
                    while (strlen(Rm)!=17)   strcat(Rm,s);
                    printf("\n%c   %d  %c %s%c %s%c %s %c%s%c%s%c%s%c %s%c  %s %c",186,buf.Tab[j].Matricule,186,nom,186,prenom,186,date,186,wilaya,186,gs,186,grade,186,fa,186,Rm,186);
                    printf("\n%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,202,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);

                }
                else
                {
                   // printf("\n\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA   %s %s  \n\n",buf.Tab[j].Nom,buf.Tab[j].Date_Naissance);
                    s[0]=' ';
                    strcpy(nom,buf.Tab[j].Nom);
                    strcpy(prenom,buf.Tab[j].Prenom);
                    strcpy(wilaya,buf.Tab[j].Wilaya_Naissance);
                    strcpy(date,buf.Tab[j].Date_Naissance);
                    strcpy(gs,buf.Tab[j].Groupe_Sanguin);
                    strcpy(grade,buf.Tab[j].Grade);
                    strcpy(fa,buf.Tab[j].Force_Armee);
                    strcpy(Rm,buf.Tab[j].Region_Militaire);
                    while (strlen(nom)<13) {strcat(nom,s);}
                    while (strlen(prenom) < 13)   { strcat(prenom,s);}
                    while (strlen(wilaya)< 17)  { strcat(wilaya,s);}
                    while (strlen(gs)<4)  {strcat(gs,s);}
                    while (strlen(grade)<25)  { strcat(grade,s);}
                    while (strlen(fa)<37) { strcat(fa,s);}
                    while (strlen(Rm)<17)  {strcat(Rm,s);}
                    printf("\n%c   %d  %c %s%c %s%c %s %c%s%c%s%c%s%c %s%c  %s %c",186,buf.Tab[j].Matricule,186,nom,186,prenom,186,date,186,wilaya,186,gs,186,grade,186,fa,186,Rm,186);
                    printf("\n%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",204,205,205,205,205,205,205,205,205,205,205,205,206,205,205,205,205,205,205,205,205,205,205,205,205,205,205,206,205,205,205,205,205,205,205,205,205,205,205,205,205,205,206,205,205,205,205,205,205,205,205,205,205,205,205,206,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,206,205,205,205,205,206,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,206,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,206,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,185);

                }
            }
            textcolor(WHITE);
        }
       textcolor(WHITE);
       printf("\n\n\t");
       return f;
}



/// ********************************************************************************************************************** ///

  void suppression_index(TOF_index *fichier, long mat)
{
    int trouv,i,j;
    BufferIndex buf1;
    rechDicho_TOF(mat, fichier,&trouv,&i,&j);
    if(trouv == 0)
    {
        //printf("\n\t\\  SUPPRESSION IMPOSSIBLE ( le ccp que vous avez entrer n'existe pas !! )  \\ \n");
        return;
    }
    else
    {
        lireDir2(fichier,i,&buf1);
        buf1.tab[j].effacement=1;
        ecrireDir2(fichier,i,&buf1);nb_e++;
       // printf("\n\t\\  SUPPRESSION REUSSITE  \\ \n");
    }
}
 void suppression_index_force(TOF_index *fichier, long mat)
{
    int trouv,i,j;
    BufferIndexForce buf1;
    rechDicho_force_TOF(mat, fichier,&trouv,&i,&j);
    if(trouv == 0)
    {
        //printf("\n\t\\  SUPPRESSION IMPOSSIBLE ( le ccp que vous avez entrer n'existe pas !! )  \\ \n");
        return;
    }
    else
    {
        lireDir2(fichier,i,&buf1);
        buf1.tab[j].effacement=1;
        ecrireDir2(fichier,i,&buf1);nb_e++;
       // printf("\n\t\\  SUPPRESSION REUSSITE  \\ \n");
    }
}


/// ********************************************************************************************************************** ///


void supp_LNOF(LNOF *F,TOF_index *fichier, long matricule)//la supression doit etre physique
{
   int trouv,i,j,N;
   Buffer buf1;
   Buffer buf2,buf3;
   Recherche_LNOF(F,fichier,matricule,&trouv,&i,&j);
   if(trouv==0)
   {
       textcolor(RED);
       printf("\n\n\n\tcet enregistrement n'existe pas dans le fichier");
       textcolor(WHITE);
   }
   else
   {
      N =entete( F, 1 );
      if(i!=N){
        lireDir(F,i,&buf1);nb_l++;
        lireDir( F,N,&buf2 );nb_l++;
        buf1.Tab[j]=buf2.Tab[buf2.nb-1];
        //buf2.Tab[buf2.nb]=buf2.Tab[buf2.nb-1];
        buf2.nb=buf2.nb-1;
        ecrireDir(F,i,&buf1);nb_e++;
        if(buf2.nb>0){
           aff_entete(F,4,entete(F,4)-1);
            ecrireDir(F,N,&buf2);nb_e++;
        }
        else{
            aff_entete(F,1,N-1);
            aff_entete(F,2,1);
            aff_entete(F,3,N-1);
            aff_entete(F,4,entete(F,4)-1);
            lireDir(F,N-1,&buf3);nb_l++;
            buf3.suivant=-1;
            ecrireDir(F,N-1,&buf3);nb_e++;
        }
      }
      else
      {
          lireDir( F, N, &buf1 );nb_l++;
          buf1.Tab[j]=buf1.Tab[buf1.nb-1];
          //buf1.Tab[buf1.nb]=buf1.Tab[buf1.nb-1];
          buf1.nb=buf1.nb-1;
          if(buf1.nb>0){
          aff_entete(F,4,entete(F,4)-1);
          ecrireDir(F,N,&buf1);nb_e++;
          }
          else{
            aff_entete(F,1,N-1);
            aff_entete(F,2,1);
            aff_entete(F,3,N-1);
            aff_entete(F,4,entete(F,4)-1);
            lireDir(F,N-1,&buf2);nb_l++;
            buf2.suivant=-1;
            ecrireDir(F,N-1,&buf2);nb_e++;
          }
      }
      textcolor(LIGHTGREEN);
      printf("\n\n\n\tla suppression a ete effectuee avec succee");
      textcolor(WHITE);
   }
}
void supp_LNOF_2(LNOF *F,TOF_index *fichier, int i,int j)//la supression doit etre physique
{
   int N,oldj;
   Buffer buf1;
   Buffer buf2,buf3;
   N =entete( F, 1 );
      if(i!=N){
        lireDir(F,i,&buf1);
        lireDir( F,N,&buf2 );
        oldj=buf2.nb-1;
        buf1.Tab[j]=buf2.Tab[buf2.nb-1];
        maj_dernier_enreg_force(F,fichier,i,j,buf2.Tab[oldj].Force_Armee,N,oldj);
        buf2.Tab[buf2.nb]=buf2.Tab[buf2.nb-1];
        buf2.nb=buf2.nb-1;
        if(buf2.nb>0){
           aff_entete(F,4,entete(F,4)-1);
            ecrireDir(F,N,&buf2);
        }
        else{
            aff_entete(F,1,N-1);
            aff_entete(F,2,1);
            aff_entete(F,3,N-1);
            aff_entete(F,4,entete(F,4)-1);
            lireDir(F,N-1,&buf3);
            buf3.suivant=-1;
            ecrireDir(F,N-1,&buf3);
        }
      }
      else
      {
          lireDir( F, N, &buf1 );
          buf1.Tab[j]=buf1.Tab[buf1.nb-1];
          oldj=buf1.nb-1;
          maj_dernier_enreg_force(F,fichier,i,j,buf1.Tab[oldj].Force_Armee,N,oldj);
          //buf1.Tab[buf1.nb]=buf1.Tab[buf1.nb-1];
          buf1.nb=buf1.nb-1;
          if(buf1.nb>0){
          aff_entete(F,4,entete(F,4)-1);
          ecrireDir(F,N,&buf1);
          }
          else{
            aff_entete(F,1,N-1);
            aff_entete(F,2,1);
            aff_entete(F,3,N-1);
            aff_entete(F,4,entete(F,4)-1);
            lireDir(F,N-1,&buf2);
            buf2.suivant=-1;
            ecrireDir(F,N-1,&buf2);
          }
      }
   }




/// ***************************************** Modification ***************************************** ///
void modification_region(LNOF *F,TOF_index *fichier)//ça consiste à faire la supp logique et physique
{
   int trouv,i,j,matricule;
   Buffer buffer;
   char reg[40];
   printf("\t\n\n");
   printf("\ndonnez le matricule de celui dont vous souhaitez changer la region melitaire : ");
   textcolor(YELLOW);
   scanf("%d",&matricule);
   textcolor(WHITE);
   Recherche_LNOF(F,fichier,matricule,&trouv,&i,&j);
   if(trouv==0)
   {
       textcolor(RED);
       printf("\n\n\n\ncet enregistrement n'existe pas dans le fichier");
       textcolor(WHITE);
   }
   else
   {
   textcolor(WHITE);
   printf("\ndonner la region melitaire :  ");
   textcolor(YELLOW);
   scanf("%s",&reg);
   lireDir(F,i,&buffer);nb_l++;
   strcpy(buffer.Tab[j].Region_Militaire,reg);
   ecrireDir(F,i,&buffer);nb_e++;
   textcolor(LIGHTMAGENTA);
   printf("\n\nVoici l'enregistrement apres modification:\n\n\n");
   textcolor(WHITE);
   printf("\n  Matricule : %d\n  Nom : %s\n  Prenom : %s",buffer.Tab[j].Matricule,buffer.Tab[j].Nom,buffer.Tab[j].Prenom);
   printf("\n  Wilaya de naissance : %s",buffer.Tab[j].Wilaya_Naissance);
   printf("\n  Le groupe sanguin : %s\n  Le grade : %s\n  Region militaire : %s\n  Force armee : %s\n  ",buffer.Tab[j].Groupe_Sanguin,buffer.Tab[j].Grade,buffer.Tab[j].Region_Militaire,buffer.Tab[j].Force_Armee);

   }
}

/**INSERTION_F**/
void Insertion_Fi(LNOF *F,TOF_index *fichier,Tenreg enreg,char Nomfichier[30])
{
    Buffer buffer;
    int n,j,nbEnregi,blocs,q,a;
    int B=(50*Max)/100;
    n = (F->entete).queue;
    lireDir(F,n,&buffer);nb_l++;
        if (buffer.nb == B)
        {
            alloc_bloc(F);
            buffer.nb = 1;
            buffer.suivant = -1;
            buffer.Tab[0]=enreg;
            q = (F->entete).queue + 1;
            ecrireDir(F,q,&buffer);nb_e++;
            blocs = (F->entete).nbBloc;
            aff_entete(F,1,blocs);
            aff_entete(F,3,q);
            /**a = n-1 ;
            lireDir(F,a,&buffer);
            buffer.suivant = q;
            ecrireDir(F,n,&buffer);**/
        }
        else
        {
            buffer.Tab[buffer.nb]=enreg;
            buffer.nb ++;
            ecrireDir(F,n,&buffer);nb_e++;
        }
        nbEnregi = (F->entete).nbEnreg +1;
        aff_entete(F,4,nbEnregi);
}
LNOF *fich_vide(LNOF *F,char Nomfichier[30])
{
    Buffer buffer;
    int i,j,k,nb,n,ne;
    int B = 20;
    F = Ouvrir(Nomfichier,'N');
    j = 0; k = 1;
    alloc_bloc(F);
    buffer.nb = j;
    buffer.suivant = -1;
    ecrireDir(F,k,&buffer);nb_e++;
    aff_entete(F,1,k);
    aff_entete(F,2,1);
    aff_entete(F,3,k);
    aff_entete(F,4,0);
    return F;
}
void fragmentation_F1(TOF_index *fich,LNOF *f,LNOF *f2, LNOF *fichier)   /** on va creer ce fichier a partir du fichier L/OVF **/
{
    int i,j,nbEnreg,nbBloc;
    int trouv2,bloc,depl;
    long matricule;
    Buffer buffer,buf,buf2;
    Tenreg enreg;
    int i1=1,j1=0,i2=1,j2=0,i3=1,j3=0,i4=1,j4=0,i5=1,j5=0,i6=1,j6=0,cpt=0;
    nbBloc = (fichier->entete).nbBloc; // pour recuperer les matricules du fichier LNOF
    for (i=1; i<= nbBloc; i++)
    {
        lireDir(fichier,i,&buffer);nb_l++;
        nbEnreg = buffer.nb;
        for (j=0; j<nbEnreg; j++)
        {
            if(strcmp("1RM-Blida",buffer.Tab[j].Region_Militaire)==0)
            {
            strcpy(enreg.Nom,buffer.Tab[j].Nom);
            strcpy(enreg.Prenom,buffer.Tab[j].Prenom);
            strcpy(enreg.Grade,buffer.Tab[j].Grade);
            strcpy(enreg.Region_Militaire,buffer.Tab[j].Region_Militaire);
            strcpy(enreg.Wilaya_Naissance,buffer.Tab[j].Wilaya_Naissance);
            strcpy(enreg.Date_Naissance,buffer.Tab[j].Date_Naissance);
            strcpy(enreg.Groupe_Sanguin,buffer.Tab[j].Groupe_Sanguin);
            strcpy(enreg.Force_Armee,buffer.Tab[j].Force_Armee);
            enreg.Matricule=buffer.Tab[j].Matricule;
            //lireDir(f,i1,&buf);
            Insertion_Fi(f,fich,enreg,"F1.bin");
            }
            if(strcmp("2RM-Oran",buffer.Tab[j].Region_Militaire)==0)
            {
            strcpy(enreg.Nom,buffer.Tab[j].Nom);
            strcpy(enreg.Prenom,buffer.Tab[j].Prenom);
            strcpy(enreg.Grade,buffer.Tab[j].Grade);
            strcpy(enreg.Region_Militaire,buffer.Tab[j].Region_Militaire);
            strcpy(enreg.Wilaya_Naissance,buffer.Tab[j].Wilaya_Naissance);
            strcpy(enreg.Date_Naissance,buffer.Tab[j].Date_Naissance);
            strcpy(enreg.Groupe_Sanguin,buffer.Tab[j].Groupe_Sanguin);
            strcpy(enreg.Force_Armee,buffer.Tab[j].Force_Armee);
            enreg.Matricule = buffer.Tab[j].Matricule;
            lireDir(f2,i2,&buf2);nb_l++;
            Insertion_Fi(f2,fich,enreg,"F2.bin");
            }
        }
    }
    Fermer(f);
    Fermer(f2);
}
void fragmentation_F2(TOF_index *fich,LNOF *f3,LNOF *f4, LNOF *fichier)   /** on va creer ce fichier a partir du fichier L/OVF **/
{
    int i,j,nbEnreg,nbBloc;
    int trouv2,bloc,depl;
    long matricule;
    Buffer buffer,buf3,buf4;
    Tenreg enreg;
    int i1=1,j1=0,i2=1,j2=0,i3=1,j3=0,i4=1,j4=0,i5=1,j5=0,i6=1,j6=0,cpt=0;
    nbBloc = (fichier->entete).nbBloc; // pour recuperer les matricules du fichier LNOF
    for (i=1; i<= nbBloc; i++)
    {
        lireDir(fichier,i,&buffer);nb_l++;
        nbEnreg = buffer.nb;
        for (j=0; j<nbEnreg; j++)
        {
            if(strcmp("3RM-Bechar,",buffer.Tab[j].Region_Militaire)==0)
            {
            strcpy(enreg.Nom,buffer.Tab[j].Nom);
            strcpy(enreg.Prenom,buffer.Tab[j].Prenom);
            strcpy(enreg.Grade,buffer.Tab[j].Grade);
            strcpy(enreg.Region_Militaire,buffer.Tab[j].Region_Militaire);
            strcpy(enreg.Wilaya_Naissance,buffer.Tab[j].Wilaya_Naissance);
            strcpy(enreg.Date_Naissance,buffer.Tab[j].Date_Naissance);
            strcpy(enreg.Groupe_Sanguin,buffer.Tab[j].Groupe_Sanguin);
            strcpy(enreg.Force_Armee,buffer.Tab[j].Force_Armee);
            enreg.Matricule = buffer.Tab[j].Matricule;
            lireDir(f3,i3,&buf3);nb_l++;
            Insertion_Fi(f3,fich,enreg,"F3.bin");
            }
            if(strcmp("4RM-Ouargla",buffer.Tab[j].Region_Militaire)==0)
            {
            strcpy(enreg.Nom,buffer.Tab[j].Nom);
            strcpy(enreg.Prenom,buffer.Tab[j].Prenom);
            strcpy(enreg.Grade,buffer.Tab[j].Grade);
            strcpy(enreg.Region_Militaire,buffer.Tab[j].Region_Militaire);
            strcpy(enreg.Wilaya_Naissance,buffer.Tab[j].Wilaya_Naissance);
            strcpy(enreg.Date_Naissance,buffer.Tab[j].Date_Naissance);
            strcpy(enreg.Groupe_Sanguin,buffer.Tab[j].Groupe_Sanguin);
            strcpy(enreg.Force_Armee,buffer.Tab[j].Force_Armee);
            enreg.Matricule = buffer.Tab[j].Matricule;
            lireDir(f4,i4,&buf4);nb_l++;
            Insertion_Fi(f4,fich,enreg,"F4.bin");
            }
        }
    }
    Fermer(f3);
    Fermer(f4);
}
void fragmentation_F3(TOF_index *fich,LNOF *f5,LNOF *f6, LNOF *fichier)   /** on va creer ce fichier a partir du fichier L/OVF **/
{
    int i,j,nbEnreg,nbBloc;
    int trouv2,bloc,depl;
    long matricule;
    Buffer buffer,buf5,buf6;
    Tenreg enreg;
    int i1=1,j1=0,i2=1,j2=0,i3=1,j3=0,i4=1,j4=0,i5=1,j5=0,i6=1,j6=0,cpt=0;
    nbBloc = (fichier->entete).nbBloc; // pour recuperer les matricules du fichier LNOF
    for (i=1; i<= nbBloc; i++)
    {
        lireDir(fichier,i,&buffer);nb_l++;
        nbEnreg = buffer.nb;
        for (j=0; j<nbEnreg; j++)
        {
            if(strcmp("5RM-Constantine",buffer.Tab[j].Region_Militaire)==0)
            {
            strcpy(enreg.Nom,buffer.Tab[j].Nom);
            strcpy(enreg.Prenom,buffer.Tab[j].Prenom);
            strcpy(enreg.Grade,buffer.Tab[j].Grade);
            strcpy(enreg.Region_Militaire,buffer.Tab[j].Region_Militaire);
            strcpy(enreg.Wilaya_Naissance,buffer.Tab[j].Wilaya_Naissance);
            strcpy(enreg.Date_Naissance,buffer.Tab[j].Date_Naissance);
            strcpy(enreg.Groupe_Sanguin,buffer.Tab[j].Groupe_Sanguin);
            strcpy(enreg.Force_Armee,buffer.Tab[j].Force_Armee);
            enreg.Matricule = buffer.Tab[j].Matricule;
            lireDir(f5,i5,&buf5);nb_l++;
            Insertion_Fi(f5,fich,enreg,"F5.bin");
            }
            if(strcmp("6RM-Tamanrasset",buffer.Tab[j].Region_Militaire)==0)
            {
            strcpy(enreg.Nom,buffer.Tab[j].Nom);
            strcpy(enreg.Prenom,buffer.Tab[j].Prenom);
            strcpy(enreg.Grade,buffer.Tab[j].Grade);
            strcpy(enreg.Region_Militaire,buffer.Tab[j].Region_Militaire);
            strcpy(enreg.Wilaya_Naissance,buffer.Tab[j].Wilaya_Naissance);
            strcpy(enreg.Date_Naissance,buffer.Tab[j].Date_Naissance);
            strcpy(enreg.Groupe_Sanguin,buffer.Tab[j].Groupe_Sanguin);
            strcpy(enreg.Force_Armee,buffer.Tab[j].Force_Armee);
            enreg.Matricule = buffer.Tab[j].Matricule;
            lireDir(f6,i6,&buf6);nb_l++;
            Insertion_Fi(f6,fich,enreg,"F6.bin");
            }
        }
   }
    Fermer(f5);
    Fermer(f6);
}

/// *************************************************** Pour l'affichage ************************************************** ///


void textcolor (int color) /* Changer la couleur du texte */
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), color + info.wAttributes / 16 * 16);
}


void textbackground(int bcolor)
{
     WORD wColor;
     //We will need this handle to get the current background attribute
     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;

     //We use csbi for the wAttributes word.
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
        //make a color form the current backgound and our forground color
          wColor = (csbi.wAttributes & 0xF) | ((bcolor  << 4 ) & 0xF0);
          SetConsoleTextAttribute(hStdOut, wColor);

     }
}


void affichage1()
{
    printf("\n\n\t\t\t\t                                     M e n u     p r i n c i p a l e              \n");
    textcolor(LIGHTRED);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Inserer un nouvel enregistrement au fichier.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(WHITE);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                               Modifier la region militaire d’un enregistrement.                             |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                              Supprimer un enregistrement donne par le matricule.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                        Supprimer tous les enregistrements relatifs a une force armee.                       |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                             Consulter des enregistrements donne par une critere.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                          Fragmentation du fichier.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Affichage de nombre de LireDir et EcrireDir.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                La representation graphique de l’organisation globale et interne des fichiers.               |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                              QUITTER LE PROGRAMME.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
}
void affichage2()
{
    printf("\n\n\t\t\t\t                                     M e n u     p r i n c i p a l e              \n");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Inserer un nouvel enregistrement au fichier.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(LIGHTRED);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                               Modifier la region militaire d’un enregistrement.                             |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(WHITE);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                              Supprimer un enregistrement donne par le matricule.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                        Supprimer tous les enregistrements relatifs a une force armee.                       |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                             Consulter des enregistrements donne par une critere.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                          Fragmentation du fichier.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Affichage de nombre de LireDir et EcrireDir.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                La representation graphique de l’organisation globale et interne des fichiers.               |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                              QUITTER LE PROGRAMME.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
}
void affichage3()
{
    printf("\n\n\t\t\t\t                                     M e n u     p r i n c i p a l e              \n");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Inserer un nouvel enregistrement au fichier.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                               Modifier la region militaire d’un enregistrement.                             |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(LIGHTRED);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                              Supprimer un enregistrement donne par le matricule.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(WHITE);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                        Supprimer tous les enregistrements relatifs a une force armee.                       |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                             Consulter des enregistrements donne par une critere.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                          Fragmentation du fichier.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Affichage de nombre de LireDir et EcrireDir.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                La representation graphique de l’organisation globale et interne des fichiers.               |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                              QUITTER LE PROGRAMME.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
}
void affichage4()
{
    printf("\n\n\t\t\t\t                                     M e n u     p r i n c i p a l e              \n");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Inserer un nouvel enregistrement au fichier.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                               Modifier la region militaire d’un enregistrement.                             |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                              Supprimer un enregistrement donne par le matricule.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(LIGHTRED);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                        Supprimer tous les enregistrements relatifs a une force armee.                       |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(WHITE);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                             Consulter des enregistrements donne par une critere.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                          Fragmentation du fichier.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Affichage de nombre de LireDir et EcrireDir.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                La representation graphique de l’organisation globale et interne des fichiers.               |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                              QUITTER LE PROGRAMME.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
}
void affichage5()
{
    printf("\n\n\t\t\t\t                                     M e n u     p r i n c i p a l e              \n");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Inserer un nouvel enregistrement au fichier.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                               Modifier la region militaire d’un enregistrement.                             |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                              Supprimer un enregistrement donne par le matricule.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                        Supprimer tous les enregistrements relatifs a une force armee.                       |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(LIGHTRED);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                             Consulter des enregistrements donne par une critere.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(WHITE);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                          Fragmentation du fichier.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Affichage de nombre de LireDir et EcrireDir.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                La representation graphique de l’organisation globale et interne des fichiers.               |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                              QUITTER LE PROGRAMME.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
}
void affichage6()
{
    printf("\n\n\t\t\t\t                                     M e n u     p r i n c i p a l e              \n");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Inserer un nouvel enregistrement au fichier.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                               Modifier la region militaire d’un enregistrement.                             |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                              Supprimer un enregistrement donne par le matricule.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                        Supprimer tous les enregistrements relatifs a une force armee.                       |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                             Consulter des enregistrements donne par une critere.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(LIGHTRED);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                          Fragmentation du fichier.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(WHITE);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Affichage de nombre de LireDir et EcrireDir.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                La representation graphique de l’organisation globale et interne des fichiers.               |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                              QUITTER LE PROGRAMME.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
}
void affichage7()
{
    printf("\n\n\t\t\t\t                                     M e n u     p r i n c i p a l e              \n");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Inserer un nouvel enregistrement au fichier.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                               Modifier la region militaire d’un enregistrement.                             |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                              Supprimer un enregistrement donne par le matricule.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                        Supprimer tous les enregistrements relatifs a une force armee.                       |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                             Consulter des enregistrements donne par une critere.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                          Fragmentation du fichier.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(LIGHTRED);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Affichage de nombre de LireDir et EcrireDir.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(WHITE);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                La representation graphique de l’organisation globale et interne des fichiers.               |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                              QUITTER LE PROGRAMME.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
}
void affichage8()
{
    printf("\n\n\t\t\t\t                                     M e n u     p r i n c i p a l e              \n");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Inserer un nouvel enregistrement au fichier.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                               Modifier la region militaire d’un enregistrement.                             |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                              Supprimer un enregistrement donne par le matricule.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                        Supprimer tous les enregistrements relatifs a une force armee.                       |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                             Consulter des enregistrements donne par une critere.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                          Fragmentation du fichier.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Affichage de nombre de LireDir et EcrireDir.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(LIGHTRED);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                La representation graphique de l’organisation globale et interne des fichiers.               |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(WHITE);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                              QUITTER LE PROGRAMME.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
}
void affichage9()
{
    printf("\n\n\t\t\t\t                                     M e n u     p r i n c i p a l e              \n");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Inserer un nouvel enregistrement au fichier.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                               Modifier la region militaire d’un enregistrement.                             |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                              Supprimer un enregistrement donne par le matricule.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                        Supprimer tous les enregistrements relatifs a une force armee.                       |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                             Consulter des enregistrements donne par une critere.                            |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                          Fragmentation du fichier.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                 Affichage de nombre de LireDir et EcrireDir.                                |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                La representation graphique de l’organisation globale et interne des fichiers.               |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(LIGHTRED);
    printf("\n\t\t\t     _____________________________________________________________________________________________________________");
    printf("\n\t\t\t    |                                                                                                             |");
    printf("\n\t\t\t    |                                              QUITTER LE PROGRAMME.                                          |");
    printf("\n\t\t\t    |_____________________________________________________________________________________________________________|");
    textcolor(WHITE);
}


void Consultation1()
{
    printf("\n\n\n\n\n");
    textcolor(YELLOW);
    printf("\n\t\t     _________________________________________________________________________________________________________________________________________");
    printf("\n\t\t    |                                                                                                                                        |");
    printf("\n\t\t    |                Consulter les enregistrements relatifs a region militaire donnee ayant un age entre deux valeurs donnees                |");
    printf("\n\t\t    |________________________________________________________________________________________________________________________________________|");
    textcolor(WHITE);
    printf("\n\t\t     ________________________________________________________________________________________________________________________________________");
    printf("\n\t\t    |                                                                                                                                        |");
    printf("\n\t\t    |                           Consulter tous les enregistrements appartenant a une categorie de grades donnee                              |");
    printf("\n\t\t    |________________________________________________________________________________________________________________________________________|");
}


void Consultation2()
{
    printf("\n\n\n\n\n");

    printf("\n\t\t     _________________________________________________________________________________________________________________________________________");
    printf("\n\t\t    |                                                                                                                                        |");
    printf("\n\t\t    |                Consulter les enregistrements relatifs a region militaire donnee ayant un age entre deux valeurs donnees                |");
    printf("\n\t\t    |________________________________________________________________________________________________________________________________________|");
    textcolor(YELLOW);
    printf("\n\t\t     ________________________________________________________________________________________________________________________________________");
    printf("\n\t\t    |                                                                                                                                        |");
    printf("\n\t\t    |                           Consulter tous les enregistrements appartenant a une categorie de grades donnee                              |");
    printf("\n\t\t    |________________________________________________________________________________________________________________________________________|");
    textcolor(WHITE);
}



void Rep1()
{
    printf("\n\n\n\n\n");
    textcolor(YELLOW);
    printf("\n\t\t     _________________________________________________________________________________________________________________________________________");
    printf("\n\t\t    |                                                                                                                                        |");
    printf("\n\t\t    |                   La representation graphique de l’organisation globale et interne du fichier 'PERSONNEL-ANP_DZ.bin.'                  |");
    printf("\n\t\t    |________________________________________________________________________________________________________________________________________|");
    textcolor(WHITE);
    printf("\n\t\t     _________________________________________________________________________________________________________________________________________");
    printf("\n\t\t    |                                                                                                                                        |");
    printf("\n\t\t    |                    La representation graphique de l’organisation globale et interne du fichier index par matricule                     |");
    printf("\n\t\t    |________________________________________________________________________________________________________________________________________|");
}


void Rep2()
{
    printf("\n\n\n\n\n");

    printf("\n\t\t     _________________________________________________________________________________________________________________________________________");
    printf("\n\t\t    |                                                                                                                                        |");
    printf("\n\t\t    |                   La representation graphique de l’organisation globale et interne du fichier 'PERSONNEL-ANP_DZ.bin.'                  |");
    printf("\n\t\t    |________________________________________________________________________________________________________________________________________|");
    textcolor(YELLOW);
    printf("\n\t\t     _________________________________________________________________________________________________________________________________________");
    printf("\n\t\t    |                                                                                                                                        |");
    printf("\n\t\t    |                    La representation graphique de l’organisation globale et interne du fichier index par matricule                     |");
    printf("\n\t\t    |________________________________________________________________________________________________________________________________________|");
    textcolor(WHITE);
}
/// ********************************************************************************************************************** ///

int main()
{
    LNOF *f2,*f3,*f4,*f5,*f6,*F,*f1;
    TOF_index *fichier,*fi,*F_force,*F_age;
    BufferIndexGrade buf;
    long matr;
    char rpnsInsertion[5];
    char rpnsSpression[5];
    char rpnsCons[5];
    int nb,choix,choix2,trouv2,bloc,depl,re,trouv4,j,p,haha,nbblocs,nbIndex;
    long matricule, MatriculeAsupprimer;


    system("cls");
    textcolor(YELLOW);
    printf("\n\n\n\n\n\t\t\t\t\t *************** Creaction du fichier PERSONNEL-ANP_DZ.bin ***************");
    textcolor(WHITE);
    printf("\n\n\t\t =========> Veuillez saisir le nombre d'enregistrements que vous voulez creer : ");
    textcolor(YELLOW);
    scanf("%d",&nb);
    textcolor(WHITE);
    F = ChargementIni(F,"PERSONNEL-ANP_DZ.bin",nb);
     printf("\n\n\n\n\n\n\n\t\t");
    system("pause");


     system("cls");
     textcolor(YELLOW);
     printf("\n\n\n\t\t\t -------> Creation des index ");
     textcolor(WHITE);
     printf("\n\n\n\t\t Apuuyer sur Entrer pour creer les index ");
     choix = getch();
     if (choix==13)
     {
        textcolor(LIGHTRED);
        printf("\n\n\t\t ** 1) Index matricule ");
        textcolor(WHITE);
        fichier = creatiaonIndex(fichier,F,"fichier.bin");
     }
     printf("\n\n\n\n\n\n\n\t\t Apuuyer encore une fois sur Entrer pour creer index 2 ");
     choix = getch();
     if (choix == 13)
     {
         textcolor(LIGHTRED);
         printf("\n\n\t\t ** 2) Index categorie grade \n\n");
         textcolor(WHITE);
         fi = creaIndexGrade(fi,F,"fichier.bin");
     }
     printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t Apuuyer encore une fois sur Entrer pour creer index 3 ");
     choix = getch();
     if (choix == 13)
     {
         textcolor(LIGHTRED);
         printf("\n\n\t\t ** 3) Index categorie force \n\n");
         textcolor(WHITE);
         F_force = creaIndexForce(F_force,F,"fichier2.bin");
     }
     printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t Apuuyer encore une fois sur Entrer pour creer index 4 ");
     choix = getch();
     if (choix == 13)
     {
         textcolor(LIGHTRED);
         printf("\n\n\t\t ** 4) Index age \n\n");
         textcolor(WHITE);
         F_age = creatiaonIndex_age(F_age,F,"fichier3.bin");
         //Affichage_TOFindex_age(F_age,"fichier3.bin");
     }
     Fermer(F);
     printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t");
     system("pause");
     system("cls");
     printf("\n\n\t\t ======> L'epuration du fichier 'suppression des doublons' \n\n");
     textcolor(WHITE);
     F = Ouvrir("PERSONNEL-ANP_DZ.bin",'A');
     supprimer_doublons(F,fichier);
     Fermer(F);
     printf("\n\n\t\tle nombre d'enregistrement est reduit de ");
     textcolor(RED);
     printf("%d",nb);
     textcolor(WHITE);
     printf(" a ");
     textcolor(RED);
     printf("%d\n\n\n\n\n\n",entete(F,4));
     textcolor(WHITE);
     system("pause");


    int touche;
    int variable=0;

touche=1;
do {system("cls");
do{
    ///affichage du menu principal
    fflush(stdin);
    switch(touche)
    {
        case 1 :
        affichage1();
        fflush(stdin);
        variable = getch();
        break;

        case 2 :
        affichage2();
        fflush(stdin);
        variable = getch();
        break;

        case 3 :
        affichage3();
        fflush(stdin);
        variable = getch();
        break;

        case 4 :
        affichage4();
        fflush(stdin);
        variable = getch();
        break;

        case 5 :
        affichage5();
        fflush(stdin);
        variable = getch();
        break;

        case 6 :
        affichage6();
        fflush(stdin);
        variable = getch();
        break;

        case 7 :
        affichage7();
        fflush(stdin);
        variable = getch();
        break;

        case 8 :
        affichage8();
        fflush(stdin);
        variable = getch();
        break;

        case 9 :
        affichage9();
        fflush(stdin);
        variable = getch();
        break;
    }

system("cls");
    if (variable==72) {if (touche==1) touche=9;else touche--;} /// machi 9 12
    else if (variable==80) {if (touche==9) touche=1;else touche++;}
    }while(variable!=13); // tant qu'on appuie pas sur entrée, on boucle


switch(touche)
{
    case 1 :
        system("cls");
        textcolor(LIGHTBLUE);
        printf("\n\n\n\t\t---------------> Insertion d'un nouvel enregistrement ");
        textcolor(WHITE);
        strcpy(rpnsInsertion,"oui");
        while ( strcmp(rpnsInsertion,"oui") == 0)
        {
            Insertion(fichier,"PERSONNEL-ANP_DZ.bin");
            Affichage_DernierBloc(F,"PERSONNEL-ANP_DZ.bin");
            printf("\n\n\t Voulez vous inserer un autre enregistrement ( oui/non ) ? : ");
            textcolor(RED);
            scanf("%s",&rpnsInsertion);
            textcolor(WHITE);
        }
        printf("\n\n\n\n\t\t");
        system("pause");
        touche=1;
        break;
    case 2 :
        system("cls");
        printf("\n\n\n\t\t---------------> Modification de la region militaire enregistrement ");
        textcolor(WHITE);
        strcpy(rpnsInsertion,"oui");
        while ( strcmp(rpnsInsertion,"oui") == 0)
        {
            F = Ouvrir("PERSONNEL-ANP_DZ.bin",'A');
            modification_region(F,fichier);
            Fermer(F);
            printf("\n\n\t Voulez vous modifier un autre enregistrement ( oui/non ) ? : ");
            textcolor(RED);
            scanf("%s",&rpnsInsertion);
            textcolor(WHITE);
        }
        printf("\n\n\n\n\t\t");
        system("pause");
        touche=2;
        break;
    case 3 :
        printf("\nsaisissez le matricule que vous voulez supprimer : ");
        scanf("%d",&matr);
        F = Ouvrir("PERSONNEL-ANP_DZ.bin",'A');
        supp_LNOF(F,fichier,matr);
        suppression_index(fichier,matr);
        Fermer(F);
        printf("\n\n\n\tvoulez vous afficher le fichier apres avoir effectue cette operation (oui/non) : ");
        textcolor(LIGHTRED);
        scanf("%s",&rpnsInsertion);
        textcolor(WHITE);
        if ( strcmp(rpnsInsertion,"oui") == 0)
        {
        printf("\n\n\n\t -------> Veuillez saisir le nombre de blocs que vous voulez afficher : ");
        textcolor(YELLOW);
        scanf("%d",&nbblocs);
        textcolor(WHITE);
        Affichage_LNOF(F,"PERSONNEL-ANP_DZ.bin");
        F = Affichage_bloc("PERSONNEL-ANP_DZ.bin",nbblocs);}
        system("pause");
        touche=3;
        break;
    case 4 :
        F = Ouvrir("PERSONNEL-ANP_DZ.bin",'A');
        Suppression_CategoriesForce("PERSONNEL-ANP_DZ.bin",F_force,fichier);
        Fermer(F);
        printf("\n\n\n\tvoulez vous afficher le fichier apres avoir effectue cette operation (oui/non) : ");
        textcolor(LIGHTRED);
        scanf("%s",&rpnsInsertion);
        textcolor(WHITE);
        if ( strcmp(rpnsInsertion,"oui") == 0)
        {
        printf("\n\n\n\t -------> Veuillez saisir le nombre de blocs que vous voulez afficher : ");
        textcolor(YELLOW);
        scanf("%d",&nbblocs);
        textcolor(WHITE);
        Affichage_LNOF(F,"PERSONNEL-ANP_DZ.bin");
        F = Affichage_bloc("PERSONNEL-ANP_DZ.bin",nbblocs);}
        system("pause");
        touche=4;
        break;
    case 5 :
        system("cls");
        int touche2=1;
        int var2;
        system("cls");
        do{
            switch (touche2){
                fflush(stdin);
            case 1:
                Consultation1();
                fflush(stdin);
                var2=getch();
                break;
            case 2:
                Consultation2();
                fflush(stdin);
                var2=getch();
                break;
            }
            system("cls");
             if (var2==72) {if (touche2==1) touche2=2;else touche2--;}
            else if (var2==80) {if (touche2==2) touche2=1;else touche2++;}
        } while (var2!=13);
        switch (touche2){
                    case 1:
                        system("cls");
                        //F = Ouvrir("PERSONNEL-ANP_DZ.bin",'A');
                        Consultation_age("PERSONNEL-ANP_DZ.bin",F_age);
                        Affichage_LNOF(F,"PERSONNEL-ANP_DZ.bin");
                        //Fermer(F);
                        system("pause");
                        break;
                    case 2:
                        system("cls");
                        strcpy(rpnsCons,"oui");
                        while ( strcmp(rpnsCons,"oui")==0)
                        {
                             Consultation_CategoriesGrades("PERSONNEL-ANP_DZ.bin",fi);
                             Affichage_LNOF(F,"PERSONNEL-ANP_DZ.bin");
                             printf("\n\n\t Voulez vous consulter autre categorie ( oui/non ) ? : ");
                             textcolor(RED);
                             scanf("%s",&rpnsCons);
                             textcolor(WHITE);
                        }
                        system("pause");
                        break;

        }
        system("pause");
        touche=5;
        break;
    case 6 :
        system("cls");
        f1=fich_vide(f1,"F1.bin");
        f2=fich_vide(f2,"F2.bin");
        f3=fich_vide(f3,"F3.bin");
        f4=fich_vide(f4,"F4.bin");
        f5=fich_vide(f5,"F5.bin");
        f6=fich_vide(f6,"F6.bin");
        F = Ouvrir("PERSONNEL-ANP_DZ.bin",'A');
        fragmentation_F1(fichier,f1,f2,F);
        fragmentation_F2(fichier,f3,f4,F);
        fragmentation_F3(fichier,f5,f6,F);
        Fermer(F);
        printf("\n\n\tveuillez choisir le numero du fichier que vous voulez afficher : ");
        scanf("%d",&choix);
        if(choix==1)
        {
        printf("\n\n\n\t -------> Veuillez saisir le nombre de blocs que vous voulez afficher : ");
        textcolor(YELLOW);
        scanf("%d",&nbblocs);
        textcolor(WHITE);
        Affichage_LNOF(f1,"f1.bin");
        F = Affichage_bloc("f1.bin",nbblocs);
        }
        if(choix==2)
        {
        printf("\n\n\n\t -------> Veuillez saisir le nombre de blocs que vous voulez afficher : ");
        textcolor(YELLOW);
        scanf("%d",&nbblocs);
        textcolor(WHITE);
        Affichage_LNOF(f2,"f2.bin");
        F = Affichage_bloc("f2.bin",nbblocs);
        }
        if(choix==3)
        {
        printf("\n\n\n\t -------> Veuillez saisir le nombre de blocs que vous voulez afficher : ");
        textcolor(YELLOW);
        scanf("%d",&nbblocs);
        textcolor(WHITE);
        Affichage_LNOF(f3,"f3.bin");
        F = Affichage_bloc("f3.bin",nbblocs);
        }
        if(choix==4)
        {
        printf("\n\n\n\t -------> Veuillez saisir le nombre de blocs que vous voulez afficher : ");
        textcolor(YELLOW);
        scanf("%d",&nbblocs);
        textcolor(WHITE);
        Affichage_LNOF(f4,"f4.bin");
        F = Affichage_bloc("f4.bin",nbblocs);
        }
        if(choix==5)
        {
        printf("\n\n\n\t -------> Veuillez saisir le nombre de blocs que vous voulez afficher : ");
        textcolor(YELLOW);
        scanf("%d",&nbblocs);
        textcolor(WHITE);
        Affichage_LNOF(f5,"f5.bin");
        F = Affichage_bloc("f5.bin",nbblocs);
        }
        if(choix==6)
        {
        printf("\n\n\n\t -------> Veuillez saisir le nombre de blocs que vous voulez afficher : ");
        textcolor(YELLOW);
        scanf("%d",&nbblocs);
        textcolor(WHITE);
        Affichage_LNOF(f6,"f6.bin");
        F = Affichage_bloc("f6.bin",nbblocs);
        }
        system("pause");
        touche=6;
        break;
    case 7 :
        F = Ouvrir("PERSONNEL-ANP_DZ.bin",'A');
        printf("\n\n\n\n\tle nombre de liredir est : ");
        textcolor(LIGHTMAGENTA);
        printf("%d",nb_l);
        textcolor(WHITE);
        printf("\n\n\tle nombre d ecriredir est : ");
        textcolor(LIGHTMAGENTA);
        printf("%d\n",nb_e);
        textcolor(WHITE);
        printf("\n\n\tle nombre d enregistrement dans le fichier est : ");
        textcolor(LIGHTMAGENTA);
        printf("%d\n",entete(F,4));
        textcolor(WHITE);
        printf("\n\n\tle nombre de bloc dans le fichier est : ");
        textcolor(LIGHTMAGENTA);
        printf("%d\n\n\n",entete(F,1));
        textcolor(WHITE);
        Fermer(F);
        system("pause");
        touche=7;
        break;
    case 8:
        system("cls");
        int touche3=1;
        int var3;
        system("cls");
        do{
            switch (touche3){
                fflush(stdin);
            case 1:
                Rep1();
                fflush(stdin);
                var3=getch();
                break;
            case 2:
                Rep2();
                fflush(stdin);
                var3=getch();
                break;
            }
            system("cls");
             if (var3==72) {if (touche3==1) touche3=2;else touche3--;}
            else if (var3==80) {if (touche3==2) touche3=1;else touche3++;}
        } while (var3!=13);
        switch (touche3){
                    case 1:
                        system("cls");
                        printf("\n\n\n\t -------> Veuillez saisir le nombre de blocs que vous voulez afficher : ");
                        textcolor(YELLOW);
                        scanf("%d",&nbblocs);
                        textcolor(WHITE);
                        Affichage_LNOF(F,"PERSONNEL-ANP_DZ.bin");
                        F = Affichage_bloc("PERSONNEL-ANP_DZ.bin",nbblocs);
                        system("pause");
                        break;
                    case 2:
                        system("cls");
                        printf("\n\n\n\t -------> Veuillez saisir le nombre de blocs que vous voulez afficher : ");
                        textcolor(YELLOW);
                        scanf("%d",&nbIndex);
                        textcolor(WHITE);
                        Affichage_TOFindex(fichier,"index.bin",nbIndex);
                        system("pause");
                        break;

        }
        system("pause");
        touche=8;
        break;
    case 9 :
        printf("\n\n\n\n\t MERCI ! \n\n\n\n\t");
        touche=9;
        break;

}

}while (touche!=9);// 12

}
