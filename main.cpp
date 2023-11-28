// Autor: Milica Djisalov RA 184/2020
// Zadatak 8: Pustinja

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>   //Omogucava upotrebu OpenGL naredbi
#include <GLFW/glfw3.h>//Olaksava pravljenje i otvaranje prozora (konteksta) sa OpenGL sadrzajem
#include <cmath>

// Time-related variables
//float currentTime = 0.0f;
//float dayDuration = 10.0f; // in seconds

unsigned int compileShader(GLenum type, const char* source); //Uzima kod u fajlu na putanji "source", kompajlira ga i vraca sejder tipa "type"
unsigned int createShader(const char* vsSource, const char* fsSource); //Pravi objedinjeni sejder program koji se sastoji od Vertex sejdera ciji je kod na putanji vsSource i Fragment sejdera na putanji fsSource

int main(void)
{

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ INICIJALIZACIJA ++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // Pokretanje GLFW biblioteke
    // Nju koristimo za stvaranje okvira prozora
    if (!glfwInit()) // !0 == 1  | glfwInit inicijalizuje GLFW i vrati 1 ako je inicijalizovana uspjesno, a 0 ako nije
    {
        std::cout<<"GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    //Odredjivanje OpenGL verzije i profila (3.3, programabilni pajplajn)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Stvaranje prozora
    GLFWwindow* window; //Mjesto u memoriji za prozor
    unsigned int wWidth = 1920;
    unsigned int wHeight = 1080;
    const char wTitle[] = "Pustinja";
    //window = glfwCreateWindow(wWidth, wHeight, wTitle, glfwGetPrimaryMonitor(), NULL);
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);

    if (window == NULL) //Ako prozor nije napravljen
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate(); //Gasi GLFW
        return 2; //Vrati kod za gresku
    }
    
    glfwMakeContextCurrent(window);
    glViewport(0, 0, wWidth, wHeight);

    // Inicijalizacija GLEW biblioteke
    if (glewInit() != GLEW_OK) //Slicno kao glfwInit. GLEW_OK je predefinisani kod za uspjesnu inicijalizaciju sadrzan unutar biblioteke
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ PROMJENLJIVE I BAFERI +++++++++++++++++++++++++++++++++++++++++++++++++

    unsigned int PyramidShader1 = createShader("pyramid1.vert", "pyramid1.frag");
    unsigned int PyramidShader2 = createShader("pyramid1.vert", "pyramid1.frag");
    unsigned int PyramidShader3 = createShader("pyramid1.vert", "pyramid1.frag");
    unsigned int FloorShader = createShader("floor.vert", "floor.frag");
    unsigned int SkyShader = createShader("sky.vert", "sky.frag");

    unsigned int VAO[10];
    glGenVertexArrays(10, VAO);
    unsigned int VBO[10];
    glGenBuffers(10, VBO);
    

    float pyramid1Vertices[] = //0.8, 0.4, 0.15
    {
        //X    Y       R    G    B    A
        -0.75, -0.2,    0.8, 0.4, 0.15, 1.0, //prvo tjeme
        -0.35, -0.2,    0.8, 0.4, 0.15, 1.0, //drugo tjeme
        -0.55,  0.2,    0.8, 0.4, 0.15, 1.0 //trece tjeme
    };

    float pyramid2Vertices[] = //0.8, 0.4, 0.15
    {
        //X    Y       R    G    B    A
        0.75, -0.2,    0.8, 0.4, 0.15, 1.0, //prvo tjeme
        0.35, -0.2,    0.8, 0.4, 0.15, 1.0, //drugo tjeme
        0.55,  0.2,    0.8, 0.4, 0.15, 1.0 //trece tjeme
    };

    float pyramid3Vertices[] = //0.8, 0.4, 0.15
    {
        //X    Y       R    G    B    A
        -0.3, -0.2,    0.8, 0.4, 0.15, 1.0, //prvo tjeme
         0.3, -0.2,    0.8, 0.4, 0.15, 1.0, //drugo tjeme
         0.0,  0.4,    0.8, 0.4, 0.15, 1.0 //trece tjeme
    };

    float floorVertices[] = //0.6, 0.3, 0.1
    {
        //X    Y       R    G    B    A
        -1.0, -1.0,    0.6, 0.3, 0.1, 1.0,
         1.0, -1.0,    0.6, 0.3, 0.1, 1.0,
         1.0, -0.2,    0.6, 0.3, 0.1, 1.0,
        -1.0, -0.2,    0.6, 0.3, 0.1, 1.0
    };

    float skyVertices[] = //0.6, 0.3, 0.1
    {
        //X    Y       R    G    B    A
        -1.0, -0.2,    0.6, 0.3, 1.0, 1.0,
         1.0, -0.2,    0.6, 0.3, 0.1, 1.0,
         1.0,  1.0,    0.6, 0.3, 0.1, 1.0,
        -1.0,  1.0,    0.6, 0.3, 0.1, 1.0
    }; 


    unsigned int stride = (2 + 4) * sizeof(float);

    // Piramida 1

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid1Vertices), pyramid1Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Piramida 2

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid2Vertices), pyramid2Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Piramida 3

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid3Vertices), pyramid3Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Pod

    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Nebo - promena dana i noci

    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ RENDER LOOP - PETLJA ZA CRTANJE +++++++++++++++++++++++++++++++++++++++++++++++++

    bool isDay = glGetUniformLocation(SkyShader, "isDay");
    bool day = true;
    bool timeRunning = true;

    while (!glfwWindowShouldClose(window)) //Beskonacna petlja iz koje izlazimo tek kada prozor treba da se zatvori
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        int currentTime = glfwGetTime();
        std::cout << currentTime;

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            timeRunning = false;
            
        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            timeRunning = true;
            currentTime = glfwGetTime();
            day = true;
        }

        if (timeRunning)
        {
            if (std::fmod(currentTime, 10) <= 5)
            {
                day = true;
            }
            else
            {
                day = false;
            }
        }
        

        // Brisanje ekrana
        glClearColor(1.0, 1.0, 1.0, 1.0); 
        glClear(GL_COLOR_BUFFER_BIT);

        // [KOD ZA CRTANJE]
        
        // Nebo - promena dana i noci
        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(SkyShader);
        glBindVertexArray(VAO[4]);
        glUniform1i(isDay, day);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // Piramida 1
        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(PyramidShader1);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Piramida 2
        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(PyramidShader2);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Piramida 3
        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(PyramidShader3);
        glBindVertexArray(VAO[2]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Pod
        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(FloorShader);
        glBindVertexArray(VAO[3]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ POSPREMANJE +++++++++++++++++++++++++++++++++++++++++++++++++


    //Brisanje bafera i sejdera
    glDeleteBuffers(1, VBO);
    glDeleteVertexArrays(1, VAO);
    glDeleteProgram(PyramidShader1);
    glDeleteProgram(PyramidShader2);
    glDeleteProgram(PyramidShader3);
    glDeleteProgram(FloorShader);
    //Sve OK - batali program
    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    //Uzima kod u fajlu na putanji "source", kompajlira ga i vraca sejder tipa "type"
    //Citanje izvornog koda iz fajla
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str(); //Izvorni kod sejdera koji citamo iz fajla na putanji "source"

    int shader = glCreateShader(type); //Napravimo prazan sejder odredjenog tipa (vertex ili fragment)
    
    int success; //Da li je kompajliranje bilo uspjesno (1 - da)
    char infoLog[512]; //Poruka o gresci (Objasnjava sta je puklo unutar sejdera)
    glShaderSource(shader, 1, &sourceCode, NULL); //Postavi izvorni kod sejdera
    glCompileShader(shader); //Kompajliraj sejder

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); //Provjeri da li je sejder uspjesno kompajliran
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog); //Pribavi poruku o gresci
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{
    //Pravi objedinjeni sejder program koji se sastoji od Vertex sejdera ciji je kod na putanji vsSource

    unsigned int program; //Objedinjeni sejder
    unsigned int vertexShader; //Verteks sejder (za prostorne podatke)
    unsigned int fragmentShader; //Fragment sejder (za boje, teksture itd)

    program = glCreateProgram(); //Napravi prazan objedinjeni sejder program

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource); //Napravi i kompajliraj vertex sejder
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource); //Napravi i kompajliraj fragment sejder

    //Zakaci verteks i fragment sejdere za objedinjeni program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program); //Povezi ih u jedan objedinjeni sejder program
    glValidateProgram(program); //Izvrsi provjeru novopecenog programa

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success); //Slicno kao za sejdere
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    //Posto su kodovi sejdera u objedinjenom sejderu, oni pojedinacni programi nam ne trebaju, pa ih brisemo zarad ustede na memoriji
    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}
