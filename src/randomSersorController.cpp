#include "RandomSensorController.h"

const char* LOG_FILE_OLD = "/sensor-history-old.csv";
const char* LOG_FILE_ACTIVE = "/sensor-history-active.csv";

// Taille maximale du journal glissant : 
// Ex: 100 KB. (Augmentez à 1024 * 1024 pour 1 Mo)
// 512 KB pour les 2 fichiers
const long MAX_LOG_SIZE = 256 * 1024;

// 1 heure en millisecondes : 1 heure * 60 min * 60 sec * 1000 ms = 3 600 000 ms
// Utiliser 'UL' (unsigned long) est crucial pour garantir la bonne gestion des grands nombres.
const unsigned long READ_INTERVAL = 5000UL; 

// Variable pour stocker le dernier moment où l'action a été exécutée
unsigned long _lastActionTime = 0;

// Fonction pour simuler la lecture d'un capteur (à remplacer par votre code réel)
float RandomSensorController::readSensorValue() {
  // Simule une lecture de température entre 20.0 et 30.0
  return random(200, 300) / 10.0;
}

// Fonction principale pour enregistrer l'entrée
void RandomSensorController::appendCircularLog(const char* data) {
  // 1. Ouvrir le fichier en mode "a+" (ajout et création si n'existe pas)
  File logFile = LittleFS.open(LOG_FILE_ACTIVE, "a+"); 
  
  if (!logFile) {
    Serial.println("ERREUR: Échec de l'ouverture du fichier de log.");
    return;
  }
    
  // 2. Vérification de la taille actuelle
  if (logFile.size() >= MAX_LOG_SIZE) {
    Serial.printf("LOG CIRCULAIRE: Taille max (%ld octets) atteinte.\n", MAX_LOG_SIZE);
    
    // Fermer le fichier
    logFile.close();
    
    // Supprimer l'ancien fichier (pour réinitialiser le journal)
    LittleFS.remove(LOG_FILE_OLD); 
    Serial.println("LOG CIRCULAIRE: Ancien fichier supprimé.");

    // Renommage du fichier actif qui devient l'ancien fichier.
    LittleFS.rename(LOG_FILE_ACTIVE, LOG_FILE_OLD);
    
    logFile = LittleFS.open(LOG_FILE_ACTIVE, "a+");
    if (!logFile) return; // Vérification rapide
  } 

  // 3. Écriture de la nouvelle entrée
  logFile.print(data);

  // 4. Fermer le fichier
  logFile.close();
}

// Fonction d'affichage du journal (pour le débogage)
void RandomSensorController::printLogFile() {
  File logFileActive = LittleFS.open(LOG_FILE_ACTIVE, "r");
  if (!logFileActive) {
    Serial.println("Le fichier de log actif n'existe pas ou est vide.");
    return;
  }

  Serial.println("\nContenu du Log actif");
  while (logFileActive.available()) {
    Serial.write(logFileActive.read());
  }
  logFileActive.close();

  File logFileOld = LittleFS.open(LOG_FILE_OLD, "r");
  if (!logFileOld) {
    Serial.println("Le fichier de log archivé n'existe pas ou est vide.");
    return;
  }

  Serial.println("\nContenu du Log archivé");
  while (logFileOld.available()) {
    Serial.write(logFileOld.read());
  }
  logFileOld.close();
}

void RandomSensorController::readSensorLoop() {

  unsigned long tempsActuel = millis();

  // Vérifier si le temps écoulé depuis la dernière action est >= à l'intervalle de lecture
  if (tempsActuel - _lastActionTime >= READ_INTERVAL) {
    _lastActionTime = tempsActuel;

    // 1. Lire les données du capteur
    float temperature = readSensorValue();
    unsigned long timestamp = millis() / 1000; // Temps écoulé en secondes

    // 2. Préparer l'entrée CSV
    String logEntry = String(timestamp) + "," + String(temperature, 1) + "\n";
    
    // 3. Enregistrer de façon circulaire
    appendCircularLog(logEntry.c_str());
    
    Serial.printf("Enregistré: %s", logEntry.c_str());
  }
}