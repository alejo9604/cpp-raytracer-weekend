#include <fstream>
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"


vec3 color_normals(const ray &r, hitable *world)
{
    hit_record rec;
    if (world->hit(r, 0.001f, MAXFLOAT, rec))
    {
        return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
    }

    // vector is now from -1 to 1
    vec3 unit_direction = unit_vector(r.direction());
    // convert that to 0 - 2 and finally scale it back down to: 0 - 1
    float t = 0.5f * (unit_direction.y() + 1.0f);
    // lerp - blended_value = (1-t)*start_value + t*end_value
    return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

vec3 color(const ray &r, hitable *world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec))
    {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return vec3(0, 0, 0);
        }
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}


hitable *random_scene()
{
    int n = 500;
    hitable **list = new hitable *[n + 1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float choose_mat = drand48();
            vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9)
            {
                if (choose_mat < 0.8)
                {  // diffuse
                    list[i++] = new sphere(
                            center, 0.2,
                            new lambertian(vec3(drand48() * drand48(),
                                                drand48() * drand48(),
                                                drand48() * drand48()))
                    );
                }
                else if (choose_mat < 0.95)
                { // metal
                    list[i++] = new sphere(
                            center, 0.2,
                            new metal(vec3(0.5 * (1 + drand48()),
                                           0.5 * (1 + drand48()),
                                           0.5 * (1 + drand48())),
                                      0.5 * drand48())
                    );
                }
                else
                {  // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list, i);
}


int main()
{
    std::ofstream outfile;
    outfile.open("../raytracer.ppm");

    int image_width = 1200;
    int image_height = 800;
    int samples_per_pixel = 10; //antialiasing
    //header image descriptor
    outfile << "P3\n" << image_width << " " << image_height << "\n255\n";

    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
//    float dist_to_focus = (lookfrom - lookat).length();
    float dist_to_focus = 10;
    float aperture = 0.1f;
    camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(image_width) / float(image_height), aperture, dist_to_focus);
//    float R = cos(M_PI / 4);

//    hitable *list[5];
//    list[0] = new sphere(vec3(0, 0, -1), 0.5f, new lambertian(vec3(0.1, 0.2, 0.5)));
//    list[1] = new sphere(vec3(0, -100.5f, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
//    list[2] = new sphere(vec3(1, 0, -1), 0.5f, new metal(vec3(0.8, 0.6, 0.2), 0.2f));
//    list[3] = new sphere(vec3(-1, 0, -1), 0.5f, new dielectric(1.5f));
//    list[4] = new sphere(vec3(-1, 0, -1), -0.45f, new dielectric(1.5f));
//    hitable *world = new hitable_list(list, 5);

    hitable *world = random_scene();

    for (int j = image_height - 1; j >= 0; j--)
    {
        std::cout << "height generated: " << j << std::endl;

        for (int i = 0; i < image_width; ++i)
        {
            vec3 col(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                float u = float(i + drand48()) / float(image_width);
                float v = float(j + drand48()) / float(image_height);
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }

            // average color from all samples
            col /= float(samples_per_pixel);
            // gamma correction
            col = vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));

            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());
            outfile << ir << " " << ig << " " << ib << "\n";
        }
    }

    outfile.close();
    return 0;
}