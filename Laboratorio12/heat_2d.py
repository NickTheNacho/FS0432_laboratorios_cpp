import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


# -----------------------------
# Parametros fisicos y numericos
# -----------------------------

Lx = 1.0
Ly = 1.0

Nx = 50
Ny = 50

kappa = 1.0

h = Lx / Nx

r = 0.20
t_final = 0.1
snapshot_interval = 20

# TODO:
# calcular

# dt

dt = r * h**2 / kappa

# num_steps

num_steps = int(round(t_final / dt))

# -----------------------------
# Condicion inicial
# -----------------------------

u = np.zeros((Nx + 1, Ny + 1))

# -----------------------------
# Condiciones de frontera
# -----------------------------

def aplicar_frontera(u):
    # TODO:
    # implementar las condiciones de frontera
    u[0, :] = 10.0
    u[Nx, :] = 10.0
    u[:, 0] = 5.0
    u[:, Ny] = 5.0

aplicar_frontera(u)


# -----------------------------
# Evolucion temporal
# -----------------------------

snapshots = []
tiempos = []

u_new = np.copy(u)

for n in range(num_steps + 1):

    # TODO:
    # guardar snapshots y tiempos

    if n % snapshot_interval == 0:
        snapshots.append(u.copy())
        tiempos.append(n * dt)

    # cuidado con copias superficiales

    # TODO:
    # completar la actualización
    
    u_new[1:Nx, 1:Ny] = u[1:Nx, 1:Ny] + r * (
    u[2:Nx+1, 1:Ny] + u[0:Nx-1, 1:Ny] +
    u[1:Nx, 2:Ny+1] + u[1:Nx, 0:Ny-1]
    - 4.0 * u[1:Nx, 1:Ny])

    # aplicar condiciones de frontera
    
    aplicar_frontera(u_new)

    # preparar arreglos u y u_new para siguiente iteración

    u, u_new = u_new, u

# -----------------------------
# Animacion
# -----------------------------

fig, ax = plt.subplots(figsize=(6, 5))

im = ax.imshow(
    snapshots[0].T,
    origin="lower",
    extent=[0, Lx, 0, Ly],
    cmap="hot",
    vmin=0.0,
    vmax=np.max(snapshots[0])
)

cbar = plt.colorbar(im, ax=ax)
cbar.set_label("Temperatura")

ax.set_xlabel("x")
ax.set_ylabel("y")
ax.set_title("Ecuacion de calor 2D")

time_text = ax.text(
    0.02,
    0.95,
    "",
    transform=ax.transAxes,
    color="white",
    fontsize=11,
    verticalalignment="top"
)


def actualizar(frame):
    im.set_data(snapshots[frame].T)
    time_text.set_text(f"t = {tiempos[frame]:.4f}")
    return im, time_text


anim = FuncAnimation(
    fig,
    actualizar,
    frames=len(snapshots),
    interval=80,
    blit=True
)

anim.save("calor_2d.gif", writer="pillow", fps=15)

plt.show()
