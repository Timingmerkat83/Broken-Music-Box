import time

# Configuration initiale
sequence = ["touch6", "touch0", "touch3"]  # La séquence attendue
current_step = 0  # Étape actuelle dans la séquence
play_start_time = None  # Moment où la musique commence à jouer

# Référence vers l'opérateur Audio File In, Constant CHOP, et Text TOP
audio_operator = op("audiofilein1")  # Nom de l'Audio File In CHOP
constant_operator = op("constant1")  # Nom du Constant CHOP
text_operator = op("text1")  # Nom du Text TOP (assurez-vous que ce nom correspond à votre Text TOP dans TouchDesigner)

# Fonction appelée quand un échantillon change dans un canal CHOP
def onValueChange(channel, sampleIndex, val, prev):
    global current_step, play_start_time

    # Si un canal non valide est activé alors que la musique joue
    if play_start_time and val == 1 and channel.name not in sequence:
        print(f"Canal incorrect '{channel.name}' détecté. Musique arrêtée.")
        stop_music()
        current_step = 0  # Réinitialise la séquence
        return

    # Vérifie si le canal en cours correspond à l'étape actuelle
    if channel.name == sequence[current_step] and val == 1:
        current_step += 1  # Passe à l’étape suivante

        # Si la séquence est complète
        if current_step == len(sequence):
            print("Porte ET activée!")  # Affiche un message dans le Textport
            play_music()  # Joue la musique
            play_start_time = time.time()  # Enregistre l'heure de début
            current_step = 0  # Réinitialise la séquence pour permettre un nouvel essai
    elif channel.name in sequence and val == 1 and channel.name != sequence[current_step]:
        # Si un canal attendu mais pas à la bonne étape est activé
        print(f"Séquence incorrecte à l'étape {current_step + 1}. Réessayer.")
        stop_music()  # Arrête la musique si elle joue
        current_step = 0  # Réinitialise la séquence pour un nouvel essai

    return

# Fonction pour jouer la musique
def play_music():
    global audio_operator, constant_operator, text_operator

    # Vérifie si l'opérateur Audio File In est valide
    if audio_operator:
        audio_operator.par.cuepulse.pulse()  # Réinitialise et joue depuis le début
        audio_operator.par.play = True  # Joue la musique
        print("Musique en cours de lecture.")
        
        # Met à jour la valeur du Constant CHOP à 1 (musique joue)
        if constant_operator:
            constant_operator.par.value0 = 1  # Remplace value0 par 1
        
        # Met à jour le texte dans le Text TOP pour afficher "Sequence Success"
        if text_operator:
            text_operator.par.text = "Sequence Success"  # Affiche le message de succès

# Fonction pour arrêter la musique
def stop_music():
    global audio_operator, play_start_time, constant_operator, text_operator

    if audio_operator:
        audio_operator.par.play = False  # Arrête la musique
        print("Musique arrêtée.")
        
        # Met à jour la valeur du Constant CHOP à 0 (musique arrêtée)
        if constant_operator:
            constant_operator.par.value0 = 0  # Remplace value0 par 0
        
        # Réinitialise le texte du Text TOP lorsque la musique est arrêtée
        if text_operator:
            text_operator.par.text = ""  # Réinitialise le texte

        play_start_time = None  # Réinitialise le temps de lecture

# Obligatoire dans un CHOP Execute DAT
def onCook(scriptOp):
    global play_start_time

    # Si la musique est en cours de lecture et que 5 secondes se sont écoulées
    if play_start_time and (time.time() - play_start_time >= 5):
        stop_music()  # Arrête la musique après 5 secondes

    return
