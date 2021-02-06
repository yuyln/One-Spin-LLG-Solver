
import matplotlib.pyplot as plt;
import matplotlib.animation as ani;

def cria(size):
        fig = plt.figure(figsize=size)
        ax = fig.add_subplot(111, projection='3d')
        return fig, ax;

def le_arq(path):
	with open(path, "r") as arq:
		linhas = arq.readlines();
		colunas = [[] for i in linhas[0].split("\t")];
		for linha in linhas:
			for i, ele in enumerate(linha.split("\t")):
				colunas[i].append(float(ele));
	return colunas;

def pegavec(i):
        global colunas;
        global field;
        x, y, z = zip([0, 0, 0], [0, 0, 0]);
        help_ = [colunas]
        u, v, w = zip([colunas[j][i] for j in range(len(colunas))], [field[j][i] for j in range(len(field))])
        return x, y, z, u, v, w;
def anima(i):
        global colunas;
        global b;
        global colors;
        b.remove();
        b = ax.quiver(*pegavec(i), linewidth=5, colors=colors);

def pega_dados(path):
	file = open(path, "r");
	linhas = file.readlines();
	n = int(linhas[1].split(" = ")[1]);
	tmax = float(linhas[3].split(" = ")[1].split(", ")[1].split(")")[0]);
	corte = int(linhas[5].split(" = ")[1])
	return n, tmax, corte;

def faz(tempo):
        fps = int(len(colunas[0]) / tempo);
        a = ani.FuncAnimation(fig, anima, frames=len(colunas[0]), interval=1);
        a.save("out.mp4", fps=fps, dpi=200);

colunas = le_arq("out.dat");
field = le_arq("field.dat");
fig, ax = cria([10, 10]);
ax.set_xlim([min(min(zip(colunas[0], field[0]))), max(max(zip(colunas[0], field[0])))]);
ax.set_ylim([min(min(zip(colunas[1], field[1]))), max(max(zip(colunas[1], field[1])))]);
ax.set_zlim([min(min(zip(colunas[2], field[2]))), max(max(zip(colunas[2], field[2])))]);
ax.set_xlabel("X");
ax.set_ylabel("Y");
ax.set_zlabel("Z");
n, tmax, corte = pega_dados("data.dat");
# vec2 vec1 vec2 vec2 vec1 vec1
colors = [(0, 0, 0), (1, 0, 0), (0, 0, 0), (0, 0, 0), (1, 0, 0), (1, 0, 0)]
b = ax.quiver(*pegavec(0), colors=colors, linewidth=5);
faz(10);
