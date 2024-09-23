import numpy as np
import requests
path = r'C:\Users\elias\Jedi\Documents\24v\passapE6000Adventure-1\test\blomabordi 1.txt'
pattern = np.loadtxt(path,dtype=int)
start = 30


r = requests.post('http://localhost:3000/api/pattern', json={
    "start":start,
    "pattern":pattern.tolist()
})