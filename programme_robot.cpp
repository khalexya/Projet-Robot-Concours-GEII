# Le Code Embarqué (en C)

La logique du robot a été entièrement programmée en C/C++ (syntaxe Arduino). Le défi n'était pas seulement de suivre une ligne, mais de naviguer un parcours complexe en autonomie.

Pour cela, a été  développé une **machine à états** qui utilise les capteurs pour se repérer.

# Décomposition du Problème

```c
// Déclaration des fonctions principales
void initialiserRobot();
int8_t readSensor(uint8_t S[]);
void motor(direction dir, uint8_t speedLeft, uint8_t speedRight);
void suivreLigne(int8_t z);
void gestionDemiTour();
void detectionIntersection(int8_t z);

// Extrait de la logique principale de navigation
// (Ceci est une machine à états simplifiée)

...
// Étape 1 : Suivre tout droit pour les deux premières bandes noires
if (compteurZ64 < 3 && z == 64) 
{
    compteurZ64++;
    motor(FRONT, 100, 100);
}

// Étape 2 : Gérer une intersection spécifique
else if (compteurZ32 == 3 && z == 32) 
{
    compteurZ32++;
    delay(250);
    motor(RIGHT, 50, 80);
    compteurZ32++;
}

// Étape 3 : Gérer le demi-tour (exemple)
else if (compteurZ64 == 4 && z == 64) 
{
    motor(STOP, 0, 0);
    delay(80);
    motor(LEFT, 25, 25);
    delay(400);
    compteurZ64++;
}
// Extrait de la fonction readSensor()
int8_t readSensor(uint8_t S[]) {
  int8_t x = 0;
  static int8_t y = 0;

  S[0] = digitalRead(5);
  S[1] = digitalRead(A2);
  ...
  S[5] = digitalRead(4);
  
  // Convertit 6 bits en un octet
  y = S[5]*1 + S[4]*2 + S[3]*4 + S[2]*8 + S[1]*16 + S[0]*32;

  // Transforme la valeur binaire en position (-5 à +5)
  switch (y) {
    case 0b100000: x = -5; break; // Totalement à droite
    case 0b001100: x = 0; break;  // Centré
    case 0b000001: x = 5; break;  // Totalement à gauche
    case 0b111111: x = 64; break; // Bande noire
    case 0b000000: x = -64; break; // Hors ligne
    default: x = -128; break;
  }
  return x;
}
