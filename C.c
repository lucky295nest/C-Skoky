#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Zavodnik {
    char prijmeni[50];
    char jmeno[50];
    char datumNarozeni[11];
    char stat[4];
    int delka;
    char tymy[4];
};

char *prevedDatum(char *ceskeDatum) {
    char *datum = (char *)malloc(11);

    int den, mesic, rok;
    sscanf(ceskeDatum, "%d.%d.%d", &den, &mesic, &rok);

    sprintf(datum, "%04d-%02d-%02d", rok, mesic, den);

    return datum;
}

int main() {
    FILE *soubor = fopen("skoky.txt", "r");

    if (soubor == NULL) {
        perror("Chyba při otevírání souboru");
        return 1;
    }

    // Přeskočení prvního řádku
    char radekNadpisu[256];
    if (fgets(radekNadpisu, sizeof(radekNadpisu), soubor) == NULL) {
        perror("Chyba při čtení prvního řádku");
        fclose(soubor);
        return 1;
    }

    struct Zavodnik zavodnici[100];
    int pocetZavodniku = 0;
    int pocetNezarazenych = 0;
    int nejlepsiSkok = 0;
    int celkovaDelka = 0;
    int pocetZavodnikuVTymu = 0;

    char radek[256];
    while (fgets(radek, sizeof(radek), soubor)) {
        sscanf(radek, "%[^;];%[^;];%[^;];%[^;];%d;%s", 
               zavodnici[pocetZavodniku].prijmeni, 
               zavodnici[pocetZavodniku].jmeno, 
               zavodnici[pocetZavodniku].datumNarozeni, 
               zavodnici[pocetZavodniku].stat, 
               &zavodnici[pocetZavodniku].delka, 
               zavodnici[pocetZavodniku].tymy);

        if (strcmp(zavodnici[pocetZavodniku].tymy, "NE") == 0) {
            pocetNezarazenych++;
        } else {
            celkovaDelka += zavodnici[pocetZavodniku].delka;
            pocetZavodnikuVTymu++;
        }

        if (zavodnici[pocetZavodniku].delka > nejlepsiSkok) {
            nejlepsiSkok = zavodnici[pocetZavodniku].delka;
        }

        pocetZavodniku++;
    }

    fclose(soubor);

    printf("Celkovy pocet zavodniku: %d\n", pocetZavodniku);
    printf("Pocet zavodniku nezarazenych v tymech: %d\n", pocetNezarazenych);
    printf("Zavodnik s nejdelsim skokem: %d\n", nejlepsiSkok);
    printf("Prumerna delka skoku zavodniku zařazených v týmech: %.2f\n", 
           (float)celkovaDelka / pocetZavodnikuVTymu);

    FILE *htmlSoubor = fopen("SKOKY.HTML", "w");
    if (htmlSoubor == NULL) {
        perror("Chyba při otevírání výstupního souboru");
        return 1;
    }

    fprintf(htmlSoubor, "<html>\n<head>\n<style>\n");
    fprintf(htmlSoubor, "table {\nborder-collapse: collapse;\nwidth: 100%%;\n}\n");
    fprintf(htmlSoubor, "th, td {\nborder: 1px solid black;\npadding: 8px;\ntext-align: left;\n}\n");
    fprintf(htmlSoubor, "tr:nth-child(even) {background-color: #f2f2f2;}\n");
    fprintf(htmlSoubor, "</style>\n</head>\n<body>\n<table>\n");
    fprintf(htmlSoubor, "<tr><th>Poradi</th><th>Jmeno</th><th>Prijmeni</th><th>Datum narozeni</th><th>Delka skoku</th></tr>\n");

    int poradi = 1;
    for (int i = 0; i < pocetZavodniku; i++) {
        if (strcmp(zavodnici[i].stat, "CZE") == 0 && strcmp(zavodnici[i].tymy, "ANO") == 0) {
            char *nejakeDatum = prevedDatum(zavodnici[i].datumNarozeni);
            fprintf(htmlSoubor, "<tr><td>%d.</td><td>%s</td><td>%s</td><td>%s</td><td>%d m</td></tr>\n",
                    poradi, zavodnici[i].jmeno, zavodnici[i].prijmeni, nejakeDatum, zavodnici[i].delka);
            free(nejakeDatum);
            poradi++;
        }
    }

    fprintf(htmlSoubor, "</table>\n</body>\n</html>\n");
    fclose(htmlSoubor);

    return 0;
}
