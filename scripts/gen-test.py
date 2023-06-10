import random
import sys



def gen_palabras(letter, n):
    """Genera n palabras de longitud aleatoria con la letra letter"""
    with open("es.txt", "r", encoding= 'UTF-8') as f:
        
        palabras = f.readlines()
        if letter != "random": # asumo que voy a usar solo 1 letra por vez, sino que haga lo de abajo
            palabras = [p.strip() for p in palabras if p[0] == letter]
            palabras = random.sample(palabras, n)
            with open(f"tests/no_random/{n}.txt", "a") as f:
                for p in palabras:
                        f.write(p + "\n")
        else:
            with open(f"tests/random/random{n}.txt", "a", encoding= 'UTF-8') as f:
                palabras = [p.strip() for p in palabras if p[0] != "ñ"]
                palabras = random.sample(palabras, n)
                for p in palabras:
                        f.write(p + "\n")



for i in range(5, 10001, 500):
    gen_palabras("a", i)
    gen_palabras("random", i)



