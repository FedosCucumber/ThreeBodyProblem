#include "Renderer.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "ShaderStorageBuffer.h"


const long double AU = 149.597870700;
const long double G =  0.0000000006743;
const long double MASS = 5972200000000000;

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
        LineShader("Basic.shader")
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
        float line_points[160] =
        {
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f
        };
        unsigned int indicies_lines[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        // Focking hell, Tommy

        glPointSize(20.0f);
        glLineWidth(10.0f);

        Body* bods = static_cast <Body*>( std::malloc(sizeof(Body) * 4));

        for (int i = 0; i < 4; i++)
        {
            new (bods + i) Body(position, indicies, indicies_lines);
        }


        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));;
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        glm::mat4 projection = glm::perspective(45.0f, 1920.0f / 1080.0f, 50.0f, -50.0f);

        glm::vec3 moving(0.0f, 0.0f, -100.0f);


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearDepth(-50.f);         // Set background depth to farthest
        glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
        glDepthFunc(GL_GREATER);   // Set the type of depth-test
        glShadeModel(GL_SMOOTH);   // Enable smooth shading
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glDepthMask(GL_TRUE);
        glDepthRange(50.0f, -50.0f);

        ShaderStorageBuffer ssbo(line_points, sizeof(float) * 4 * 40);

        ssbo.Unbind();


        float colors[16] =
        {
            0.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f
        };

        glm::vec3 pos[4]
        {
            glm::vec3(0.0f,  0.0f,  10.0f),
            glm::vec3(20.0f, 0.0f,  20.0f),
            glm::vec3(5.0f,  10.0f, -10.0f),
            glm::vec3(10.0f, 10.0f, -20.0f),
        };

        glm::vec3 speed[4]
        {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
        };

        float mass[4] =
        {
            1.0f, 10.0f, 1.0f, 1.0f
        };



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
                ImGui::SliderFloat("Move", &moving.z, -100.0f, -50.0f);
                ImGui::SliderFloat2("Move", &moving.x, -100.0f, 100.0f);
            }


            glm::vec3 new_pos[4] = { pos[0], pos[1], pos[2], pos[3] };

            for (int i = 0; i < 4; ++i)
            {
                glm::vec3 force(0.0f, 0.0f, 0.0f);
                for (int j = 0; j < 4; j++)
                {
                   // std::cout << glm::length(pos[i] - pos[j]) << std::endl;
                    auto dist3 = glm::length(pos[i] - pos[j]) * glm::length(pos[i] - pos[j]) * glm::length(pos[i] - pos[j]) + 0.1f;
                    if (dist3) {
                        force += (pos[i] - pos[j]) *  float(-1.0f / dist3) * 30.0f * mass[j];
                    }
                }

                speed[i] += force * (1.0f / 60.0f);

                new_pos[i] = pos[i] + speed[i] * (1.0f / 60);
            
                bods[i].BodyShader.Bind();
                bods[i].BodyShader.SetUniform4f("u_Color", colors[i * 4], colors[i * 4 + 1], colors[i * 4 + 2], colors[i * 4 + 3]);

                ssbo.Bind();
                glm::mat4 temp = glm::translate(glm::mat4(1.0f), moving + new_pos[i]);
                bods[i].BodyShader.SetUniformMat4f("u_MVP", projection * view * model * temp);
                bods[i].BodyShader.SetUniform1i("u_Index", i);

                bods[i].BodyArr.Bind();
                bods[i].BodyIb.Bind();

                glDrawElements(GL_POINTS, bods[i].BodyIb.GetCount(), GL_UNSIGNED_INT, nullptr);

                bods[i].BodyShader.Unbind();
                bods[i].BodyArr.Unbind();
                bods[i].BodyIb.Unbind();

                bods[i].LineShader.Bind();
                bods[i].LinesIb.Bind();
                bods[i].LineShader.SetUniform4f("u_Color", colors[i * 4], colors[i * 4 + 1], colors[i * 4 + 2], colors[i * 4 + 3]);
                bods[i].LineShader.SetUniform1i("u_Index", i);


                glDrawElements(GL_LINE_STRIP, bods[i].LinesIb.GetCount(), GL_UNSIGNED_INT, nullptr);

                bods[i].LineShader.Unbind();
                bods[i].LinesIb.Unbind();
            }


            pos[0] = new_pos[0];
            pos[1] = new_pos[1];
            pos[2] = new_pos[2];
            pos[3] = new_pos[3];


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