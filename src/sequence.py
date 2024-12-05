# Assurez-vous d'attacher ce script à un CHOP Execute DAT
# Connectez-y un CHOP contenant les channels: touch6, touch0, touch3

# Configuration initiale
sequence = ["touch6", "touch0", "touch3"]  # La séquence attendue
current_step = 0  # Étape actuelle dans la séquence

# Fonction appelée quand un échantillon change dans un canal CHOP
def onValueChange(channel, sampleIndex, val, prev):
    global current_step

    # Vérifie si le canal en cours correspond à l'étape actuelle
    if channel.name == sequence[current_step] and val == 1:
        current_step += 1  # Passe à l'étape suivante

        # Si la séquence est complète
        if current_step == len(sequence):
            print("Porte ET activée!")
            # Réinitialise la séquence pour une nouvelle détection
            current_step = 0
    elif channel.name in sequence and val == 1 and channel.name != sequence[current_step]:
        # Si un canal non attendu est activé, réinitialise
        print("Séquence incorrecte, réinitialisation.")
        current_step = 0

    return

# Obligatoire dans un CHOP Execute DAT
def onCook(scriptOp):
    return
