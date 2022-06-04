#include "Renderer.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <vector>
#include <random>

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "ShaderStorageBuffer.h"



class Body
{
public:
    VertexArray BodyArr;
    IndexBuffer BodyIb;
    VertexBufferLayout layout;
    VertexBuffer BodyVb;

    IndexBuffer LinesIb;

    Shader BodyShader;
    Shader LineShader;

    Body(const void* pos, const unsigned int* Body, const unsigned int* Lines) :
        BodyArr(),
        BodyIb(Body, 1),
        layout(),
        BodyVb(pos, 1 * 3 * sizeof(float)),
        LinesIb(Lines, 10),
        BodyShader("BodyShader.shader"),
        LineShader("Trajectory.shader")
    {
        layout.Push <float>(3);

        BodyArr.Bind();
        BodyVb.Bind();
        BodyArr.AddBuffer(BodyVb, layout);

        BodyArr.Unbind();
        BodyVb.Unbind();
    }
};

int main(void)
{


    unsigned n;
    std::cout << "Number of bodies to be created: ";
    std::cin >> n;

   

    GLFWwindow* window;
    //GLCheckError();


    /* Initialize the library */
    if (!glfwInit())
        return -1;




    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glewInit();

    glfwSwapInterval(1);

    {
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();
        while (GLenum error = glGetError())
        {
            std::cout << "[OpenGL error] (" << error << ") " <<
                " " << __FILE__ << ": " << __LINE__ << std::endl;
        }
        float position[3] = { 0.0f, 0.0f, 0.0f };
        unsigned int indicies[1] = { 0 };
        glm::vec4* line_points = static_cast <glm::vec4*> (std::malloc(n * 10 * sizeof(glm::vec4)));
        for (unsigned i = 0; i < n * 10; ++i)
        {
            line_points[i] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        }

        unsigned int indicies_lines[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        glPointSize(20.0f);
        glLineWidth(10.0f);

        float colors[32] =
        {
            0.0f,   0.0f,  0.0f,   1.0f,
            1.0f,   0.0f,  0.0f,   1.0f,
            0.0f,   1.0f,  0.0f,   1.0f,
            0.0f,   0.0f,  1.0f,   1.0f,
            0.95f,  0.02f, 1.0f,   1.0f, 
            1.0f,   1.0f,  0.02f,  1.0f,
            1.0f,   0.5f,  0.02f,  1.0f,
            0.0f,   1.0f,  1.0f,   1.0f,
        };

        Body* bods = static_cast <Body*>( std::malloc(sizeof(Body) * n));

        for (int i = 0; i < n; i++)
        {
            new (bods + i) Body(position, indicies, indicies_lines);

            bods[i].BodyShader.Bind();
            bods[i].BodyShader.SetUniform4f("u_Color", colors[(i % 8) * 4], colors[(i % 8) * 4 + 1], colors[(i % 8) * 4 + 2], colors[(i % 8) * 4 + 3]);
            bods[i].BodyShader.SetUniform1i("u_Index", i);
            bods[i].BodyShader.Unbind();
            bods[i].LineShader.Bind();
            bods[i].LineShader.SetUniform4f("u_Color", colors[(i % 8) * 4], colors[(i % 8) * 4 + 1], colors[(i % 8) * 4 + 2], colors[(i % 8) * 4 + 3]);
            bods[i].LineShader.SetUniform1i("u_Index", i);
            bods[i].LineShader.Unbind();
        }


        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));;
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        glm::mat4 projection = glm::perspective(45.0f, 1920.0f / 1080.0f, 50.0f, -50.0f);

        glm::vec3 moving(0.0f, 0.0f, -120.0f);


        GLcall(glEnable(GL_BLEND));                                    // Enable blending
        GLcall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));     // Set blending function
        GLcall(glClearDepth(-50.f));                                   // Set background depth to farthest
        GLcall(glEnable(GL_DEPTH_TEST));                               // Enable depth testing for z-culling
        GLcall(glDepthFunc(GL_GREATER));                               // Set the type of depth-test
        GLcall(glShadeModel(GL_SMOOTH));                               // Enable smooth shading
        GLcall(glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST));     // Set perspective correction ppolicy. 
        GLcall(glDepthMask(GL_TRUE));                                  // Enable writing to depth buffer

        ShaderStorageBuffer ssbo(line_points, n * 10 * sizeof(glm::vec4));

        ssbo.Unbind();




        std::random_device dev;
        std::mt19937 engine(dev());
        std::uniform_int_distribution <int> distr(-90, 90);




        std::vector <glm::vec3> pos(n);

        for (auto& vec : pos)
        {
            vec.x = distr(engine);
            vec.y = distr(engine);
            vec.z = distr(engine) % 10;
        }
        std::vector <glm::vec3> speed(n);
        std::vector <float> mass(n);
        for (auto& _Mass : mass)
        {
            _Mass = std::abs(distr(engine)) + 1;
        }



        /* Loop until the user closes the window */

        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClearDepth(-50.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ImGui_ImplGlfwGL3_NewFrame();
            {
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::SliderFloat("Move", &moving.z, -300.0f, -50.0f);
                ImGui::SliderFloat2("Move", &moving.x, -100.0f, 100.0f);
            }


            std::vector <glm::vec3> new_pos = pos;

            for (int i = 0; i < n; ++i)
            {
                glm::vec3 force(0.0f, 0.0f, 0.0f);
                for (int j = 0; j < n; j++)
                {
                    auto dist3 = glm::length(pos[i] - pos[j]) * glm::length(pos[i] - pos[j]) * glm::length(pos[i] - pos[j]) + 1.0f;
                    if (dist3) {
                        force += (pos[i] - pos[j]) *  float(-1.0f / dist3) * 30.0f * mass[j];
                    }
                }

                speed[i] += force * (1.0f / 60.0f);

                new_pos[i] = pos[i] + speed[i] * (1.0f / 60);
            
                bods[i].BodyShader.Bind();

                ssbo.Bind();
                glm::mat4 temp = glm::translate(glm::mat4(1.0f), moving + new_pos[i]);
                bods[i].BodyShader.SetUniformMat4f("u_MVP", projection * view * model * temp);

                bods[i].BodyArr.Bind();
                bods[i].BodyIb.Bind();

                glDrawElements(GL_POINTS, bods[i].BodyIb.GetCount(), GL_UNSIGNED_INT, nullptr);

                bods[i].BodyShader.Unbind();
                bods[i].BodyArr.Unbind();
                bods[i].BodyIb.Unbind();

                bods[i].LineShader.Bind();
                bods[i].LinesIb.Bind();
                


                glDrawElements(GL_LINE_STRIP, bods[i].LinesIb.GetCount(), GL_UNSIGNED_INT, nullptr);

                bods[i].LineShader.Unbind();
                bods[i].LinesIb.Unbind();
            }


            pos = new_pos;


            // Make a loop, faggot

            ssbo.Unbind();
            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}