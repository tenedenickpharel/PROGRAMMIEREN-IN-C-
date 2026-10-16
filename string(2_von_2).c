#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Compilerschalter: -Wall -fsanitize=address

char * aufgabe1_int2str(int zahl)
{
    int length = (zahl <= 0) ? 1 : 0;
    int temp = zahl;
    while (temp != 0) {
        length++;
        temp /= 10;
    }

    char *result = (char*)malloc((length + 1) * sizeof(char));
    if (result == NULL) {
        return NULL;
    }

    int written = sprintf(result, "%d", zahl);

    if (written > length) {
        free(result);
        return NULL;
    }

    return result;
}

long aufgabe2_str2int(const char *str)
{
    char *endptr;
    long value = strtol(str, &endptr, 10);

    if (endptr == str) {
        fprintf(stderr, "Fehler: Der String '%s' konnte nicht in eine Ganzzahl umgewandelt werden.\n", str);
        return -1;
    }

    return value;

    // ** was bewirkt der Prefix L hinter der 0 **/
    // Antwort: Das Suffix 'L' teilt dem Compiler mit, dass es sich um ein Literal vom Typ 'long int' handelt.

    // 2b) Verhalten bei NoneDigits im String
    // Antwort: Wenn der String nach gültigen Ziffern nicht-numerische Zeichen enthält (z. B. "34ab"),
    // wandelt strtol() den gültigen numerischen Teil ("34") um und stoppt die Analyse beim ersten ungültigen Zeichen.
    // Die Adresse dieses Zeichens ('a') wird im Zeiger `endptr` gespeichert.

    // 2c) Vorteil von strtol() gegenüber atol()
    // Antwort: Im Gegensatz zu atol() bietet strtol() eine robuste Fehlerbehandlung.
    // Durch `endptr` lässt sich erkennen, wo die Konvertierung abgebrochen wurde.
    // Zudem signalisiert strtol() Overflow/Underflow über die globale Variable `errno` (ERANGE).
}

char *aufgabe3_strreplace(const char *str, const char *replace)
{
    if (str == NULL || replace == NULL) return NULL;

    const char *placeholder = "$1";
    int placeholder_len = strlen(placeholder);
    int replace_len = strlen(replace);

    int count = 0;
    const char *temp = str;
    while ((temp = strstr(temp, placeholder)) != NULL) {
        count++;
        temp += placeholder_len;
    }

    size_t new_len = strlen(str) + (replace_len - placeholder_len) * count + 1;

    char *result = (char*)malloc(new_len);
    if (result == NULL) return NULL;

    char *dst = result;
    const char *src = str;
    const char *next_placeholder;

    while ((next_placeholder = strstr(src, placeholder)) != NULL) {
        size_t len_to_copy = next_placeholder - src;
        memcpy(dst, src, len_to_copy);
        dst += len_to_copy;

        memcpy(dst, replace, replace_len);
        dst += replace_len;

        src = next_placeholder + placeholder_len;
    }

    strcpy(dst, src);

    return result;
}

void aufgabe4_rtrim(char *str)
{
    if (str == NULL) return;

    int len = strlen(str);
    if (len == 0) return;

    for (int i = len - 1; i >= 0; i--) {
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r') {
            str[i] = '\0';
        } else {
            break;
        }
    }

    // 4b) Begründen sie den Laufzeitfehler bei einer der Eingaben
    // Antwort: Testfall 4c (`char *str3="   With Spaces  ";`) führt zu einem Laufzeitfehler (Segmentation Fault),
    // weil `str3` auf ein String-Literal zeigt, das in einem schreibgeschützten Speicherbereich liegt.
    // Der Versuch, diesen Speicher in `aufgabe4_rtrim` zu ändern, führt zu einer Zugriffsverletzung.
}

int main(int argc, char *argv[])
{
    char aufgabe;
    if(argc>1) {
        aufgabe=argv[1][0];
    }
    else
        aufgabe='1';

    switch(aufgabe) {
        case '1': {
            printf("Aufabe '1'\n");
            char *str_ptr;
            //Testfall 1a
            str_ptr=aufgabe1_int2str(123456789);
            if(str_ptr!=NULL)
                printf("1. Zahl='%s'\n",str_ptr),
                // ** was bewirkt das Komma am Ende von printf **/
                // Antwort: Das ist der Komma-Operator in C. Er ermöglicht es, `printf` und `free(str_ptr)`
                // nacheinander im if-Zweig auszuführen, ohne einen Block mit `{}` erstellen zu müssen.
                free(str_ptr);

            //Testfall 1b
            str_ptr=aufgabe1_int2str(0715);
            if(str_ptr!=NULL) {
                printf("2. Zahl='%s'\n",str_ptr);
                free(str_ptr);
            }

            //Testfall 1c
            str_ptr=aufgabe1_int2str(0);
            printf("3. Zahl='%s'\n",str_ptr?:"---");
            // ** was bewirkt der ?-Operator (insb. aufgrund des fehlenden mittleren Operanden) **/
            // Antwort: Dies ist eine GNU-C-Erweiterung. Er gibt den ersten Operanden zurück, falls er wahr (ungleich NULL) ist.
            // Ist `str_ptr` gültig, wird er ausgegeben, sonst "---".

            str_ptr?free(str_ptr):(void)0;
            // ** warum ist (void)0 hier als 3. Operand sinnvoll **/
            // Antwort: Der ternäre Operator erfordert, dass der zweite und dritte Operand kompatible Typen haben.
            // `free()` gibt `void` zurück. `(void)0` bewirkt nichts, hat aber den Typ `void`, was die Syntax korrekt macht.
            break;
        }
        case '2':
            printf("Aufabe '2'\n");
            printf("1.Int=%ld\n",aufgabe2_str2int("1234"));
            printf("2.Int=%ld\n",aufgabe2_str2int("   -4711"));
            printf("3.Int=%ld\n",aufgabe2_str2int("34ab"));
            printf("4.Int=%ld\n",aufgabe2_str2int("a123"));
            printf("5.Int=%ld\n",aufgabe2_str2int("hallo"));
            break;
        case '3': {
            printf("Aufabe '3'\n");
            const char *eingabe[4][2]={
                {"Der String $1 hat keine Bedeutung","a"},
                {"Der String $1 hat keine Bedeutung","'123456789'"},
                {"$1 ersetzt $1"                    ,"abc"},
                {"$1"                               ,"abc"},
            };
            for(size_t lauf=0; lauf<4; lauf++) {
                char *str_ptr=aufgabe3_strreplace(eingabe[lauf][0],eingabe[lauf][1]);
                if(str_ptr != NULL) {
                    printf("%zu. String='%s'\n",lauf,str_ptr);
                    free(str_ptr);
                }
            }
            break;
        }
        case '4': {
            printf("Aufabe '4'\n");
            char str1[]="String \n  a\t  \r";
            printf("Orignal='%s' ",str1);
            aufgabe4_rtrim(str1);
            printf("Rtrim='%s'\n",str1);

            char str2[30];
            strcpy(str2,"Without Whitespaces");
            printf("Orignal='%s' ",str2);
            aufgabe4_rtrim(str2);
            printf("Rtrim='%s'\n",str2);

            char *str3="   With Spaces  ";
            printf("Orignal='%s' ",str3);
            aufgabe4_rtrim(str3);
            printf("Rtrim='%s'\n",str3);
            }
            break;
        default:
            fprintf(stderr,"Ungültige Aufgabenwahl\n");
            break;
    }

    return 0;
    // ** über welchen OS-Funktion kann alternativ das Programm beendet werden **/
    // Antwort: Über die Funktion `exit(0);` (aus <stdlib.h>) oder `_exit(0);` (POSIX, aus <unistd.h>).

    // ** Wer nimmt diesen Rückgabewert entgegeben **/
    // Antwort: Das Betriebssystem bzw. der aufrufende Prozess (die Shell). Unter Linux abrufbar mit `$?`.
}