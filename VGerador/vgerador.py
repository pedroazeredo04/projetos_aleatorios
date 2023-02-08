from random import randint

v_words = open("docs/palavras_v.txt").readlines()
g_words = open("docs/palavras_g.txt").readlines()

v_chosen = v_words[randint(0, len(v_words) - 1)]
g_chosen = g_words[randint(0, len(g_words) - 1)]

print(f'VG significa: {v_chosen.strip()} {g_chosen.strip()}')
