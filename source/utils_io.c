#include <utils_io.h>
#include <stdlib.h>

void saveParticles(struct CALModel3D *ca, CALint step, CALreal elapsed_time, double CPU_time, char* path)
{
  FILE* f;
  f = fopen(path, "w");

  if ( !f )
    {
      printf ("Unable to open %s.\n", path);
      exit(EXIT_FAILURE);
    }

  CALint number_of_particles = 0;
  CALreal total_energy = 0.0;

  CALreal p[3], v[3], F[3];
  CALint particle_id;


  fprintf(f, "Current step: %d\n", step);
  fprintf(f, "Physical elapsed time: %.9f\n", elapsed_time);
  fprintf(f, "CPU time: %.9f\n", CPU_time);
  fprintf(f, "id     \tr[0]     \tr[1]     \tr[2]     \tv[0]     \tv[1]     \tv[2]     \tF[0]     \tF[1]     \tF[2]\n");

  for (int cell_x=0; cell_x<ca->rows; cell_x++)
    for (int cell_y=0; cell_y<ca->columns; cell_y++)
      for (int cell_z = 0; cell_z<ca->slices; cell_z++)
        for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
          if (calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z) > NULL_ID)
            {
              F[0] = calGet3Dr(ca, Q.Fx[slot],cell_x,cell_y,cell_z);
              F[1] = calGet3Dr(ca, Q.Fy[slot],cell_x,cell_y,cell_z);
              F[2] = calGet3Dr(ca, Q.Fz[slot],cell_x,cell_y,cell_z);

              p[0] = calGet3Dr(ca, Q.px[slot],cell_x,cell_y,cell_z);
              p[1] = calGet3Dr(ca, Q.py[slot],cell_x,cell_y,cell_z);
              p[2] = calGet3Dr(ca, Q.pz[slot],cell_x,cell_y,cell_z);

              v[0] = calGet3Dr(ca, Q.vx[slot],cell_x,cell_y,cell_z);
              v[1] = calGet3Dr(ca, Q.vy[slot],cell_x,cell_y,cell_z);
              v[2] = calGet3Dr(ca, Q.vz[slot],cell_x,cell_y,cell_z);

              particle_id = calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z);

              number_of_particles++;
              total_energy += PARTICLE_MASS*G*p[2];
              total_energy += 0.5*PARTICLE_MASS*v[0]*v[0];
              total_energy += 0.5*PARTICLE_MASS*v[1]*v[1];
              total_energy += 0.5*PARTICLE_MASS*v[2]*v[2];

              fprintf(f, "%d\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\n", particle_id, p[0], p[1], p[2], v[0], v[1], v[2], F[0], F[1], F[2]);
            }

   fprintf(f, "Total number of particles: %d\n", number_of_particles);
   fprintf(f, "Total energy: %.9f\n", total_energy);

   fclose(f);
}
