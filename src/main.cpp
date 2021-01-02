#include "Helpers.h"
#include <iostream>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>
#else
// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>
#endif
// OpenGL Mathematics Library
#include <glm/glm.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/transform.hpp>
#include <chrono> //Timer

using namespace std;

// VertexBufferObject wrapper
VertexBufferObject VBO;
VertexBufferObject VBO_color;

// Contains the vertex positions
std::vector<glm::vec2> V(15);
std::vector<glm::vec3> V_Color(15);
std::vector<glm::vec2> Temp(2);
std::vector<glm::vec3> Temp_Color(2);
std::vector<glm::vec2> Temp_tr(3);
std::vector<glm::vec3> Temp_tr_Color(3);
std::vector<glm::vec2> Animation_V1(15);
std::vector<glm::vec3> Animation_C1(15);
std::vector<glm::vec2> Animation_V2(15);
std::vector<glm::vec3> Animation_C2(15);
std::vector<glm::vec2> V_Keyframe(15);
std::vector<glm::vec3> C_Keyframe(15);

int cases = 1;
int index = 0;
int index_sel=-1;
int count = 0;
int Vertex_select, Vertex_selected;
double vx1,vx2,vx3,vy1,vy2,vy3,vmx,vmy;
double r1,r2,r3,b1,b2,b3,g1,g2,g3;
double View_Scale=1.0, X_shift=0.0, Y_shift=0.0;
float timeValue, timeAnimation;

glm::mat3 cross_product(glm::mat3 mat1, glm::mat3 mat2) {
    glm::mat3 result;

    for(int i=0 ; i<3 ; i++) {
        for(int j=0 ; j<3 ; j++) {
            result[i][j] = 0;
        }
    }

    for(int i=0 ; i<3 ; i++) {
        for(int j=0 ; j<3 ; j++) {
            for(int k=0 ; k<3 ; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    return result;
}
//Checks if the xworld and yworld recorded are in the traingle
bool pt_in_triangle(double x1, double x2, double x3, double y1, double y2, double y3, double x, double y)
{
    float a = ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3)) / ((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
    float b = ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3)) / ((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
    float c = 1 - a - b;
    bool a_val = a>=0 && a<=1;
    bool b_val = b>=0 && b<=1;
    bool c_val = c>=0 && c<=1;
    return (a_val && b_val && c_val);
}
// Defines window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
//Tells what happens after every click
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Get the position of the mouse in the window
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    // Get the size of the window
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    // Convert screen position to world coordinates
    double xworld = ((((xpos/double(width))*2)-1)-X_shift)/View_Scale;
    double yworld = (((((height-1-ypos)/double(height))*2)-1)-Y_shift)/View_Scale; 
    
    bool isIn=false;
    
    // Update the position of the first vertex if the left button is pressed
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        switch (cases)
        {
            case 1:  //Nothing is selected
                cout<<"No mode selected. To insert a tringle press I."<<endl;
                break;
            case 2: // One point is fixed
                /*
                    [x1, y1]
                 */
                if(index < V.size()) {
                    Temp[0] = glm::vec2(xworld, yworld);
                    Temp_Color[0] = glm::vec3(1.0, 1.0, 1.0);
                    cases = 3;
                    
                }else {
                    cout << "Delete atleast one traingle....." << endl;
                }
                break;
            case 3:  // The first line is fixed
                /*
                    [x1, y1         [r1, g1, b1
                     x2, y2]         r2, g2, b2]

                 */
                Temp_tr[0] = glm::vec2(Temp[0].x, Temp[0].y);
                Temp_tr[1] = glm::vec2(xworld, yworld);

                Temp_tr_Color[0] = glm::vec3(1.0, 1.0, 1.0);
                Temp_tr_Color[1] = glm::vec3(1.0, 1.0, 1.0);
                cases=4;
                break;
            case 4: //makes the triangles and loops the process
                V[index] = glm::vec2(Temp_tr[0].x, Temp_tr[0].y);
                V[index+1] = glm::vec2(Temp_tr[1].x, Temp_tr[1].y);
                V[index+2] = glm::vec2(xworld, yworld);
                count += 3;

                V_Color[index] = glm::vec3(1.0, 1.0, 1.0);
                V_Color[index+1] = glm::vec3(1.0, 1.0, 1.0);
                V_Color[index+2] = glm::vec3(1.0, 1.0, 1.0);
                index = index + 3;
                cases=2;
                break;
            case 7:
            case 5: //Translation
                for(int i=0;i<V.size();i+=3)
                {
                    if(pt_in_triangle(V[i].x,V[i+1].x,V[i+2].x,V[i].y,V[i+1].y,V[i+2].y,xworld, yworld))
                    {
                        isIn=true;
                        if(index_sel!=-1)
                        {
                            V_Color[index_sel] = glm::vec3(r1,g1,b1);
                            V_Color[index_sel+1] = glm::vec3(r2,g2,b2);
                            V_Color[index_sel+2] = glm::vec3(r3,g3,b3);
                        }
                        index_sel=i;
                        vx1=V[i].x;   vy1=V[i].y;
                        vx2=V[i+1].x; vy2=V[i+1].y;
                        vx3=V[i+2].x; vy3=V[i+2].y;
                        vmx=xworld; vmy=yworld;
                        r1=V_Color[i].x;  g1=V_Color[i].y;  b1=V_Color[i].z;
                        r2=V_Color[i+1].x;   g2=V_Color[i+1].y;    b2=V_Color[i+1].z;
                        r3=V_Color[i+2].x;   g3=V_Color[i+2].y;    b3=V_Color[i+2].z;
                        cases=6;
                        break;
                    }
                    if(!isIn)
                    {
                        if(index_sel!=-1)
                        {
                            V_Color[index_sel] = glm::vec3(r1,g1,b1);
                            V_Color[index_sel+1] = glm::vec3(r2,g2,b2);
                            V_Color[index_sel+2] = glm::vec3(r3,g3,b3);
                        }
                        index_sel = -1;
                    }
                }
                break;
            case 8: //Deletion
                {
                    int temp = 0;
                    for(int i=0;i<(V.size()-1);i+=3)
                    {
                        if(pt_in_triangle(V[i].x,V[i+1].x,V[i+2].x,V[i].y,V[i+1].y,V[i+2].y,xworld, yworld))
                        {
                            temp = i;
                            break;
                        }
                    }
                    for(int i=temp ; i<V.size()-3 ; i++) {
                        V[i] = glm::vec2(V[i+3].x, V[i+3].y);
                        V_Color[i] = glm::vec3(V_Color[i+3].x, V_Color[i+3].y, V_Color[i+3].z);
                    }
                    V[V.size()] = glm::vec2();
                    V[V.size()-1] = glm::vec2();
                    V[V.size()-2] = glm::vec2();
                    V_Color[V_Color.size()] = glm::vec3();
                    V_Color[V_Color.size()-1] = glm::vec3();
                    V_Color[V_Color.size()-2] = glm::vec3();
                    index -= 3;
                }
                break;
            case 9: //Color mode
                Vertex_selected=Vertex_select;
                cases=10;
                cout<<"Vertex Selected to color:"<<Vertex_selected<<endl;
                break;           
            default:
                break;
        }
    }
    {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        switch(cases)
            case 6:
        {
                cases=7;
                break;
        }
    }

    // Updating the changes in the GPU
    VBO.update(V);
    VBO_color.update(V_Color);
}
//Updates the function we are on by keyboard keys
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    double alpha=3.14*10/180;
    double cx,cy;
    glm::mat3 a,b,c,Rotate_after,Rotate_Clock,Translation1,Translation2,ScaleBig,ScaleSmall,Scale_after;
    if(action==GLFW_PRESS)
    {
        if(key != GLFW_KEY_H && key != GLFW_KEY_J && key != GLFW_KEY_K && key != GLFW_KEY_L && index_sel != -1)
        {
            V_Color[index_sel] = glm::vec3(r1,g1,b1);
            V_Color[index_sel+1] = glm::vec3(r2,g2,b2);
            V_Color[index_sel+2] = glm::vec3(r3,g3,b3);
            index_sel = -1;
        }
        switch (key)
        {
            case  GLFW_KEY_I:
                cout << "Creating the traingle...." << endl;
                cases=2;
                break;
            case  GLFW_KEY_O:
                cout << "Selecting the traingle...." << endl;
                cases=5;
                break;
            case  GLFW_KEY_P:
                cout<<"Deleting the selected triangle..."<<endl;
                cases=8;
                break;
            case  GLFW_KEY_H:
                if(index_sel==-1)
                {
                    cout<<"Press O to select triangle!"<<endl;
                    break;
                }
                cout<<"Rotating the triangle clockwise.."<<endl;
                alpha=-alpha;
                /* [x1 x2 x3
                    y1 y2 y3]
                */
                cx = (V[index_sel].x + V[index_sel+1].x + V[index_sel+2].x)/3;
                cy = (V[index_sel].y + V[index_sel+1].y + V[index_sel+2].y)/3;
                for(int i=0 ; i<3 ; i++) {
                    a[0][i] = V[index_sel+i].x;
                    a[1][i] = V[index_sel+i].y;
                    a[2][i] = 1;
                }
                for(int i=0 ; i<3 ; i++) {
                    for(int j=0 ; j<3 ; j++) {
                        if(i == j) {
                            Translation1[i][j] = 1;
                            Translation2[i][j] = 1;
                        }else {
                            Translation1[i][j] = 0;
                            Translation2[i][j] = 0;
                        }
                    }
                }
                Translation1[0][2] = -cx;
                Translation1[1][2] = -cy;
                Translation2[0][2] = cx;
                Translation2[1][2] = cy;
                Rotate_Clock[0][0] = Rotate_Clock[1][1] = cos(alpha);
                Rotate_Clock[0][1] = -sin(alpha);
                Rotate_Clock[1][0] = sin(alpha);
                Rotate_Clock[0][2] = Rotate_Clock[1][2] = Rotate_Clock[2][0] = Rotate_Clock[2][1] = 0;
                Rotate_Clock[2][2] = 1;
                
                b = cross_product(Translation1, a);
                c = cross_product(Rotate_Clock, b);
                Rotate_after = cross_product(Translation2, c);
                
                for(int i=0 ; i<3 ; i++) {
                    V[index_sel+i] = glm::vec2(Rotate_after[0][i], Rotate_after[1][i]);
                }              
                break;
            case  GLFW_KEY_J:
                if(index_sel==-1)
                {
                    cout<<"Press O to select triangle!"<<endl;
                    break;
                }
                cout<<"Rotating your triangle Anti-clockwise.."<<endl;
                /* [x1 x2 x3
                    y1 y2 y3]
                */
                cx = (V[index_sel].x + V[index_sel+1].x + V[index_sel+2].x)/3;
                cy = (V[index_sel].y + V[index_sel+1].y + V[index_sel+2].y)/3;
                for(int i=0 ; i<3 ; i++) {
                    a[0][i] = V[index_sel+i].x;
                    a[1][i] = V[index_sel+i].y;
                    a[2][i] = 1;
                }
                for(int i=0 ; i<3 ; i++) {
                    for(int j=0 ; j<3 ; j++) {
                        if(i == j) {
                            Translation1[i][j] = 1;
                            Translation2[i][j] = 1;
                        }else {
                            Translation1[i][j] = 0;
                            Translation2[i][j] = 0;
                        }
                    }
                }
                Translation1[0][2] = -cx;
                Translation1[1][2] = -cy;
                Translation2[0][2] = cx;
                Translation2[1][2] = cy;
                Rotate_Clock[0][0] = Rotate_Clock[1][1] = cos(alpha);
                Rotate_Clock[0][1] = -sin(alpha);
                Rotate_Clock[1][0] = sin(alpha);
                Rotate_Clock[0][2] = Rotate_Clock[1][2] = Rotate_Clock[2][0] = Rotate_Clock[2][1] = 0;
                Rotate_Clock[2][2] = 1;
                
                b = cross_product(Translation1, a);
                c = cross_product(Rotate_Clock, b);
                Rotate_after = cross_product(Translation2, c);

                for(int i=0 ; i<3 ; i++) {
                    V[index_sel+i] = glm::vec2(Rotate_after[0][i], Rotate_after[1][i]);
                }
                break;
            case  GLFW_KEY_K:
                if(index_sel==-1)
                {
                    cout<<"Press O to select triangle!"<<endl;
                    break;
                }
                cout<<"Scaling up your triangle by 25%.."<<endl;
                cx = (V[index_sel].x + V[index_sel+1].x + V[index_sel+2].x)/3;
                cy = (V[index_sel].y + V[index_sel+1].y + V[index_sel+2].y)/3;
                for(int i=0 ; i<3 ; i++) {
                    a[0][i] = V[index_sel+i].x;
                    a[1][i] = V[index_sel+i].y;
                    a[2][i] = 1;
                }
                for(int i=0 ; i<3 ; i++) {
                    for(int j=0 ; j<3 ; j++) {
                        if(i == j) {
                            Translation1[i][j] = 1;
                            Translation2[i][j] = 1;
                            ScaleBig[i][j] = 1;
                        }else {
                            Translation1[i][j] = 0;
                            Translation2[i][j] = 0;
                            ScaleBig[i][j] = 0;
                        }
                    }
                }
                Translation1[0][2] = -cx;
                Translation1[1][2] = -cy;
                Translation2[0][2] = cx;
                Translation2[1][2] = cy;
                /* ScaleBig << 1.25, 0, 0,
                            0, 1.25, 0,
                            0,0,1; */
                ScaleBig[0][0] = ScaleBig[1][1] = 1.25;
                
                b = cross_product(Translation1, a);
                c = cross_product(ScaleBig, b);
                Scale_after = cross_product(Translation2, c);
                for(int i=0 ; i<3 ; i++) {
                    V[index_sel+i] = glm::vec2(Scale_after[0][i], Scale_after[1][i]);
                }
                break;
            case  GLFW_KEY_L:
                if(index_sel==-1)
                {
                    cout<<"Press O to select triangle!"<<endl;
                    break;
                }
                cout<<"Scaling down your triangle by 25%.."<<endl;
                cx = (V[index_sel].x + V[index_sel+1].x + V[index_sel+2].x)/3;
                cy = (V[index_sel].y + V[index_sel+1].y + V[index_sel+2].y)/3;
                for(int i=0 ; i<3 ; i++) {
                    a[0][i] = V[index_sel+i].x;
                    a[1][i] = V[index_sel+i].y;
                    a[2][i] = 1;
                }
                for(int i=0 ; i<3 ; i++) {
                    for(int j=0 ; j<3 ; j++) {
                        if(i == j) {
                            Translation1[i][j] = 1;
                            Translation2[i][j] = 1;
                            ScaleSmall[i][j] = 1;
                        }else {
                            Translation1[i][j] = 0;
                            Translation2[i][j] = 0;
                            ScaleSmall[i][j] = 0;
                        }
                    }
                }
                Translation1[0][2] = -cx;
                Translation1[1][2] = -cy;
                Translation2[0][2] = cx;
                Translation2[1][2] = cy;
                /* ScaleSmall << 0.75, 0, 0,
                            0, 0.75, 0,
                            0,0,1; */
                ScaleSmall[0][0] = ScaleSmall[1][1] = 0.75;
                
                b = cross_product(Translation1, a);
                c = cross_product(ScaleSmall, b);
                Scale_after = cross_product(Translation2, c);

                for(int i=0 ; i<3 ; i++) {
                    V[index_sel+i] = glm::vec2(Scale_after[0][i], Scale_after[1][i]);
                }
                break;
            case  GLFW_KEY_C:
                    if(index_sel != -1)
                    {
                        V_Color[index_sel] = glm::vec3(r1,g1,b1);
                        V_Color[index_sel+1] = glm::vec3(r2,g2,b2);
                        V_Color[index_sel+2] = glm::vec3(r3,g3,b3);
                    }
                    cout<<"Coloring your closest vertex.."<<endl<<"Press any number from 1-9 for a surprise color"<<endl;
                    cases=9;
                    break;
                case  GLFW_KEY_1:
                    if(cases==10)
                    {
                        V_Color[Vertex_selected] = glm::vec3(0.0,0.0,1.0);
                        cases=9;   
                    }
                    break;
                case  GLFW_KEY_2:
                    if(cases==10)
                    {
                        V_Color[Vertex_selected] = glm::vec3(0.0,1.0,0.0);
                        cases=9;
                    }
                    break;
                case  GLFW_KEY_3:
                    if(cases==10)
                    {
                        V_Color[Vertex_selected] = glm::vec3(1.0,0.0,0.0);
                        cases=9;   
                    }
                    break;
                case  GLFW_KEY_4:
                    if(cases==10)
                    {
                        V_Color[Vertex_selected] = glm::vec3(1.0,1.0,0.0);
                        cases=9;   
                    }
                    break;
                case  GLFW_KEY_5:
                    if(cases==10)
                    {
                        V_Color[Vertex_selected] = glm::vec3(0.0,1.0,1.0);
                        cases=9;   
                    }
                    break;
                case  GLFW_KEY_6:
                    if(cases==10)
                    {
                        V_Color[Vertex_selected] = glm::vec3(1.0,0.5,0.5);
                        cases=9;   
                    }
                    break;
                case  GLFW_KEY_7:
                    if(cases==10)
                    {
                        V_Color[Vertex_selected] = glm::vec3(1.0,0.0,0.5);
                        cases=9;   
                    }
                    break;
                case  GLFW_KEY_8:
                    if(cases==10)
                    {
                        V_Color[Vertex_selected] = glm::vec3(0.5,1.0,0.5);
                        cases=9;   
                    }
                    break;
                case  GLFW_KEY_9:
                    if(cases==10)
                    {
                        V_Color[Vertex_selected] = glm::vec3(0.5,0.7,1.0);
                        cases=9;   
                    }
                    break;
                case GLFW_KEY_KP_ADD:
                case GLFW_KEY_EQUAL:
                    cout<<"Zooming in.."<<endl;
                    View_Scale=View_Scale*1.2;
                    break;
                case GLFW_KEY_KP_SUBTRACT:
                case GLFW_KEY_MINUS:
                    cout<<"Zooming out.."<<endl;
                    View_Scale=View_Scale*0.8;
                    break;
                case  GLFW_KEY_W:
                    cout<<"Panning up.."<<endl;
                    Y_shift=Y_shift-0.4;
                    break;
                case  GLFW_KEY_A:
                    cout<<"Panning left.."<<endl;
                    X_shift=X_shift+0.4;
                    break;
                case  GLFW_KEY_S:
                    cout<<"Panning down.."<<endl;
                    Y_shift=Y_shift+0.4;
                    break;
                case  GLFW_KEY_D:
                    cout<<"Panning right.."<<endl;
                    X_shift=X_shift-0.4;
                    break;
                case GLFW_KEY_COMMA:
                    cout << "Selecting keyframe 1."<<endl;
                    if(Animation_V1.size()!=0)
                    {
                        V = Animation_V1;
                        V_Color = Animation_C1;
                    }
                    break;
                case GLFW_KEY_PERIOD:
                    cout<< "Switching to keyframe 2.."<< endl;
                    Animation_V1 = V;
                    Animation_C1 = V_Color;
                    Animation_V2 = V;
                    Animation_C2 = V_Color;
                    break;
                case GLFW_KEY_SPACE:
                    if(Animation_V2.size()!=V.size())
                    {
                        cout << "Make keyframe 2 to animate by pressing the Period key."<<endl;
                        break;
                    }
                    if(cases==11||cases==12)
                    {
                        V = Animation_V2;
                        V_Color = Animation_C2;
                        cases=1;
                    }
                    else
                    {
                        if(V.size()!=Animation_V1.size())
                        {
                            cout<<"Press Comma to revert to keyframe 1 and continue.."<<endl;
                            break;
                        }
                        Animation_V2 = V;
                        Animation_C2 = V_Color;
                        for(int i=0 ; i<Animation_V1.size() ; i++) {
                            V_Keyframe[i] = Animation_V2[i] - Animation_V1[i];
                        }
                        for(int i=0 ; i<Animation_C1.size() ; i++) {
                            C_Keyframe[i] = Animation_C2[i] - Animation_C1[i];
                        }
                        cout << V_Keyframe[0].x,V_Keyframe[0].y;
                        cases=11;
                        timeAnimation=timeValue;
                    }
                    break;
                default:
                    break;      
        }
    }
    // Updating the changes in the GPU
    VBO.update(V);
    VBO_color.update(V_Color);
}

int main(void)
{
    GLFWwindow* window;
    // Initialize the library
    if (!glfwInit())
        return -1;
    // Activate supersampling
    glfwWindowHint(GLFW_SAMPLES, 8);
    // Ensure that we get at least a 3.2 context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    // On apple we have to load a core profile with forward compatibility
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "Diksha's Assignment 2", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    // Make the window's context current
    glfwMakeContextCurrent(window);
    #ifndef __APPLE__
    glewExperimental = true;
    GLenum err = glewInit();
    if(GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    glGetError(); // pull and savely ignonre unhandled errors like GL_INVALID_ENUM
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    #endif

    int major, minor, rev;
    major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
    printf("OpenGL version recieved: %d.%d.%d\n", major, minor, rev);
    printf("Supported OpenGL is %s\n", (const char*)glGetString(GL_VERSION));
    printf("Supported GLSL is %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Initialize the VAO
    // A Vertex Array Object (or VAO) is an object that describes how the vertex
    // attributes are stored in a Vertex Buffer Object (or VBO). This means that
    // the VAO is not the actual object storing the vertex data,
    // but the descriptor of the vertex data.
    VertexArrayObject VAO;
    VAO.init();
    VAO.bind();

    // Initialize the VBO with the vertices data
    // A VBO is a data container that lives in the GPU memory
    VBO.init();
    VBO_color.init();
    
    VBO.update(V);
    VBO_color.update(V_Color);
    
    // Initialize the OpenGL Program
    // A program controls the OpenGL pipeline and it must contains
    // at least a vertex shader and a fragment shader to be valid
    Program program;
    const GLchar* vertex_shader =
            "#version 150 core\n"
                    "in vec2 position;"
                    "in vec3 color;"
                    "out vec3 color_out;"
                    "uniform vec3 Pan;"
                    "in vec2 Rotate_val;"
                    "in vec2 Translate_val;"
                    "in vec2 Scale_val;"
                    "mat3 Pan_pos=mat3("
                                        "position.x, position.y, 1.0,"
                                        "0.0, 0.0, 1.0,"
                                        "0.0, 0.0, 1.0"
                                        ");"
                    "mat3 View_Pan=mat3("
                                        "Pan[0], 0.0, 0.0,"
                                        "0.0, Pan[0], 0.0,"
                                        "Pan[1], Pan[2], 0.0"
                                        ");"
                    "mat3 Final=mat3(View_Pan* Pan_pos);"
                    //"mat3 rotate_mat = rotate(vec2 Rotate_val, 10, vec3(1.0,0.0,0.0)"
                    
                    "void main()"
                    "{"
                        "color_out=color;"
                        //"mat3 resultMatrix = mat3(rotate_mat * Final);"
                        "gl_Position = vec4(Final[0].x, Final[0].y, 0.0, 1.0);"
                    "}";
    const GLchar* fragment_shader =
            "#version 150 core\n"
                    "in vec3 color_out;"
                    "out vec4 outColor;"
                    "void main()"
                    "{"
                    "    outColor = vec4(color_out, 1.0);"
                    "}";

    // Compile the two shaders and upload the binary to the GPU
    // Note that we have to explicitly specify that the output "slot" called outColor
    // is the one that we want in the fragment buffer (and thus on screen)
    program.init(vertex_shader,fragment_shader,"outColor");
    program.bind();

    // The vertex shader wants the position of the vertices as an input.
    // The following line connects the VBO we defined above with the position "slot"
    // in the vertex shader
    program.bindVertexAttribArray("position",VBO);
    program.bindVertexAttribArray("color",VBO_color);
    
    // Save the current time --- it will be used to dynamically change the triangle color
    auto t_start = std::chrono::high_resolution_clock::now();
    // Register the keyboard callback
    glfwSetKeyCallback(window, key_callback);
    // Register the mouse callback
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // Update viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Bind your VAO (not necessary if you have only one)
        VAO.bind();
        // Bind your program
        program.bind();
        // Set the uniform value depending on the time difference
        auto t_now = std::chrono::high_resolution_clock::now();
        float timeValue = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
        glUniform3f(program.uniform("Pan"),View_Scale,X_shift,Y_shift);
        
        // Clear the framebuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //glUniform3f(program.uniform("triangleColor"), (float)(sin(timeValue * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        // Get the size of the window
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        // Convert screen position to world coordinates
        double xworld = ((((xpos/double(width))*2)-1)-X_shift)/View_Scale;
        double yworld = (((((height-1-ypos)/double(height))*2)-1)-Y_shift)/View_Scale; 
    
        int i;
        double delta_x=0; double delta_y=0;
        double distSq,distSqMin=(double)INT_MAX,xc1,yc1;
        int minDistSqIndex=-1;

        switch(cases)
        {
            case 3:
                Temp[1] = glm::vec2(xworld, yworld);
                Temp_Color[1] = glm::vec3(1.0, 1.0, 1.0);
                break;
            case 4:
                Temp_tr[2] = glm::vec2(xworld, yworld);
                Temp_tr_Color[2] = glm::vec3(1.0, 1.0, 1.0);
                break;
            case 6: //Translation
                delta_x = xworld-vmx; delta_y = yworld-vmy;
                V[index_sel] = glm::vec2(vx1+delta_x, vy1+delta_y);
                V[index_sel+1] = glm::vec2(vx2+delta_x, vy2+delta_y);
                V[index_sel+2] = glm::vec2(vx3+delta_x, vy3+delta_y);
                V_Color[index_sel] = glm::vec3(0.0,0.0,(float)(sin(timeValue * 8.0f)));
                V_Color[index_sel+1] = glm::vec3(0.0,0.0,(float)(sin(timeValue * 8.0f)));
                V_Color[index_sel+2] = glm::vec3(0.0,0.0,(float)(sin(timeValue * 8.0f)));
                break;
            case 9:
                for(i=0;i<V.size();i++)
                {
                    xc1=V[i].x; yc1=V[i].y; 
                    distSq=(xc1-xworld)*(xc1-xworld) + (yc1-yworld)*(yc1-yworld);
                    if(distSq < distSqMin)
                    {
                        distSqMin = distSq;
                        minDistSqIndex = i;
                    }
                }
                Vertex_select= minDistSqIndex;
                break;
            case 11:
                if(timeValue-timeAnimation>3)
                {
                    cases=12;
                    timeAnimation=timeValue;
                }
                else
                {
                    for(int i=0 ; i<Animation_V1.size() ; i++) {
                        V[i].x = Animation_V1[i].x + V_Keyframe[i].x * (timeValue-timeAnimation)/3;
                        V[i].y = Animation_V1[i].y + V_Keyframe[i].y * (timeValue-timeAnimation)/3;
                    }
                    for(int i=0 ; i<Animation_C1.size() ; i++) {
                        V_Color[i].x = Animation_C1[i].x + C_Keyframe[i].x * (timeValue-timeAnimation)/3;
                        V_Color[i].y = Animation_C1[i].y + C_Keyframe[i].y * (timeValue-timeAnimation)/3;
                        V_Color[i].z = Animation_C1[i].z + C_Keyframe[i].z * (timeValue-timeAnimation)/3;
                    }
                }
                break;
            case 12:
                if(timeValue-timeAnimation>3)
                {
                    cases=11;
                    timeAnimation=timeValue;
                }
                else
                {
                    for(int i=0 ; i<Animation_V2.size() ; i++) {
                        V[i].x = Animation_V2[i].x + V_Keyframe[i].x * (timeValue-timeAnimation)/3;
                        V[i].y = Animation_V2[i].y + V_Keyframe[i].y * (timeValue-timeAnimation)/3;
                    }
                    for(int i=0 ; i<Animation_C2.size() ; i++) {
                        V_Color[i].x = Animation_C2[i].x + C_Keyframe[i].x * (timeValue-timeAnimation)/3;
                        V_Color[i].y = Animation_C2[i].y + C_Keyframe[i].y * (timeValue-timeAnimation)/3;
                        V_Color[i].z = Animation_C2[i].z + C_Keyframe[i].z * (timeValue-timeAnimation)/3;
                    }
                }
                break;
            default:
                break;
        }
        if(index_sel != -1)
        {
            V_Color[index_sel] = glm::vec3(0.0,0.0,(float)(sin(timeValue * 8.0f)));
            V_Color[index_sel+1] = glm::vec3(0.0,0.0,(float)(sin(timeValue * 8.0f)));
            V_Color[index_sel+2] = glm::vec3(0.0,0.0,(float)(sin(timeValue * 8.0f)));
        }
        // Draw a triangle
        VBO.update(V);
        VBO_color.update(V_Color);
        glDrawArrays(GL_TRIANGLES, 0, V.size());

        VBO.update(Temp);
        VBO_color.update(Temp_Color);
        glDrawArrays(GL_LINES, 0, Temp.size());

        VBO.update(Temp_tr);
        VBO_color.update(Temp_tr_Color);
        glDrawArrays(GL_TRIANGLES, 0, Temp_tr.size());

        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }
    // Deallocate opengl memory
    program.free();
    VAO.free();
    VBO.free();
    VBO_color.free();
    // Deallocate glfw internals
    glfwTerminate();
    return 0;
}