from sys import stdin
from pulp import LpProblem, LpMaximize, LpVariable, LpStatus, value, GLPK_CMD, lpSum

data = stdin.read().splitlines()
N, M, T = map(int, data[0].split())

fabricas = {}
paises = {}
criancas = {}
variaveis = {}

index = 1
for _ in range(N):
    id_fabrica, pais_fabrica, stock = map(int, data[index].split())
    if stock > 0:
        fabricas[id_fabrica] = {"pais":pais_fabrica, "stock":stock, "var":[]}
    index += 1

for _ in range(M):
    id_pais, maxExportacao, minEntregue = map(int, data[index].split())
    paises[id_pais] = {"max":maxExportacao, "min":minEntregue, "expo": [], "entr": []}
    index += 1

for _ in range(T):
    crianca_data = list(map(int, data[index].split()))
    id_crianca = crianca_data[0]
    pais_crianca = crianca_data[1]
    brinquedos_preferidos = [f for f in crianca_data[2:] if f in fabricas]
    if brinquedos_preferidos:
        criancas[id_crianca] = {"pais": pais_crianca, "var": []}
        for brinquedo in brinquedos_preferidos:
            var = LpVariable(f"x_{id_crianca}_{brinquedo}", cat="Binary")
            variaveis[(id_crianca, brinquedo)] = var
            criancas[id_crianca]["var"].append(var)
            fabricas[brinquedo]["var"].append(var)
            paises[pais_crianca]["entr"].append(var)
            if pais_crianca != fabricas[brinquedo]["pais"]:
                paises[fabricas[brinquedo]["pais"]]["expo"].append(var)
    index += 1



prob = LpProblem("XmasSnail", LpMaximize)

prob += lpSum(variaveis.values())

for crianca in criancas.values():
    prob += lpSum(crianca["var"]) <= 1

for fabrica in fabricas.values():
    prob += lpSum(fabrica["var"]) <= fabrica["stock"]

for pais in paises.values():
    prob += lpSum(pais["entr"]) >= pais["min"]
    prob += lpSum(pais["expo"]) <= pais["max"]

prob.solve(GLPK_CMD(msg=False))

val = -1

if LpStatus[prob.status] == 'Optimal':
    val = int(value(prob.objective))

print(val)