
input_channels = ["out2", "out3", "out4"]  # Nom des canaux d'entrée
output_channel = "output"  # Le nom du canal de sortie
inputs_state = [0, 0, 0]  
def onValueChange(channel, sampleIndex, val, prev):
    global inputs_state
    if channel.name in input_channels:
        idx = input_channels.index(channel.name)
        inputs_state[idx] = val
        output_value = 1 if all(state == 1 for state in inputs_state) else 0
        set_output(output_value)
        print(f"Entrées : {inputs_state} | Sortie : {output_value}")

def set_output(value):
    output_op = op(output_channel)  # Assurez-vous que le nom de l'opérateur de sortie est correct
    if output_op:
        output_op.par.value0 = value  # Définit la valeur du canal de sortie (ex: value0)

def onCook(scriptOp):
    return
