#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <thread>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace std;
using namespace ImGui;

ImGuiIO* io;

namespace imgui_detail{
  
  // types
  static void imgui_types(){
    ImDrawList* drawList;   // command list per window
    ImDrawData* drawData;   // comamnd in list
    ImFontAtlas* fontAtlas; // list of font
    ImFont*     font;   // font in atlas
    ImColor*    color;  // helper to create color
    ImFontConfig* fontConfig; // add font or merge helper
    ImGuiContext* guiContext; // imgui context
    ImGuiPayload* dragDropPayLoad; // drag-drop payload
    ImGuiStorage* storage; // key-value storage
    ImGuiTextBuffer* textBuffer; // text manipualion buffer
    ImGuiTextFilter* textFilter; // use to filter texts - regex
    ImTextureID* textureId; // idendify texture
    ImGuiID* id;    // each widget has an id
    ImWchar* wchar; // for each keyboard char - entered ( unicode )
    ImGuiCol* colorEnum; // color enums for style
    ImVec2* vec2; // vector<2>
    ImVec4* vec4; // vector<4>
    ImGuiWindowFlags *windowFlags; // for each window - how to manage it
    ImGuiInputTextFlags *textFlags; // called but Imgui::InputText
    ImGuiSelectableFlags *selectableFlags; // selectable() flag
    ImGuiComboFlags* comboFlag; //comboFlag for BeginCombo
    ImGuiTabBarFlags* tabbarFlag; // beginTabBar flag
    ImGuiStyleVar* styleVar; // PushStyleVar() / PopStyleVar() için kullanılır.
    ImGuiTabItemFlags* tabFlag; // tabItem flag BeginTabItem()
    ImGuiFocusedFlags* focusedFlag; // focused flag for IsWindowFocused
    ImGuiHoveredFlags* hoverFlag; // IsItemHovered IsWindowHovered ... checkler için kullanılır.
    ImGuiKey* key; // key codes for keyboard but ImGuiIO.KeyMap[] KeysDown[].. bunlara bakmalı
    ImGuiNavInput *navigation; // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard ... ile keyboard array fill edilir
    ImGuiCol* pushCol; // pushstylecolor or popstylecolor
    ImGuiCond* condEnum; // SetWindow...,SetNextWİndow... kuralları ( Always,Once...firstuseage)
    
    ImGuiStyle* style; // runtime data for style/color - newFrame öncesi style güncellemesi yapabilirsin. window,frame,border,alising,antialiasing...
    
    ImGuiIO* io;    // imgui - app comm configuration interface.mouse,keyboard,size,
    
  }
  
  // display back - front
  
  // Calc item text size - width
  
  // context
  
  // frame
  
  // item rect size
  
  // content, window content
  
  // render* - many render commands for what ?
  
  // pushstylecolor ? popstylecolor ? bunlar nedir nasıl kullanılır ?
  
  // PushStyleVar() / PopStyleVar() nedir nasıl kullanılırlar ?
}

namespace basics{
  
  struct window_{
    std::string title { "window"};
    bool open { true };
  } window;
  
  //window
  static void draw_window(){
    if(window.open){
        if(ImGui::Begin(window.title.data(),&window.open,ImGuiWindowFlags_AlwaysAutoResize))
          ImGui::End();
      }
  }
  
  // Label textbox
  
  static void draw_label(){
    if(ImGui::Begin("label_test",nullptr)){
        ImGui::Text("label_test_1");
        ImGui::Text("label test_2");
        ImGui::Text("label test_3");
        ImGui::Text("label test_4");
        
        ImGui::End();
      }
  }
  
  // Tooltip
  static void draw_tooltip(){
    //ImGui::BeginChild("##tooltips",ImVec2(200,100),false,ImGuiWindowFlags_AlwaysAutoResize);
    
    ImGui::Text("Hover over me");
    if (ImGui::IsItemHovered())
      ImGui::SetTooltip("I am a tooltip");
    
    ImGui::Button("Hover over me",ImVec2(100,30));
    
    if (ImGui::IsItemHovered())
      {
        ImGui::BeginTooltip();
        ImGui::Text("I am a fancy tooltip");
        static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
        ImGui::PlotLines("Curve", arr, IM_ARRAYSIZE(arr));
        ImGui::EndTooltip();
      }
    
     //ImGui::EndChild();
  }
  
  // Mouse
  
  // Keyboard
  
  // column, row
  
  // content
  
  // Combobox
  
  // Button
  
  // Toggle
  
  // Checkbox
  
  // Groupbox
  
  // Slider
  
  // Progressbar
  
  // Image
  
  // Imagebutton
  
  // selectable
  
  static void render(){
    bool open = true;
    
    if(ImGui::Begin("##basics")){
        draw_tooltip();
        ImGui::End();
      }
  }
}

namespace complex{
  
  // groupbox
  
  // layout
  
  // Toolbar
  
  // Menubar
  
  // tabbar
  
  // colordialog
  
  // radiobutton
  
  // bullet
  
  // TreeView
  
  // Listbox
  
  // NavItem
  
  // TreeNodeEx
  
  // CollapsingHeader
}

namespace operations{
  // sameline , same row ?
  
  // same column
  
  // what is screen ?
  
  // what is window ?
  
  // color convertion
  
  // drag drop between 2 panel
  
  // window draw list
  
  // find window by id
  
  // move mouse cursor
  
  // push id - pop id
  
  // hide item
  
  // show item
  
  // toggle item
  
  // show sub window
  
  // show popup - messagebox
  
  // find size
  
  // update listview
  
  // udpate treeview
  
  // show panel
  
  // hide panel
  
  // update properties of nextpanel - which kind of properties ?
  
  // how to change properties nextpanel
  
  // what kind of ways to update next ?
  
  // allow app to capture mouse or imgui ? how ? io.WantCaptureMouse
  
}

namespace window{
  // is entered
  
  // is hovered
  
  // is clicked
  
  // is focused
  
  // is closed
}



static void glfw_error_callback(int error, const char* description)
{
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}




int main(int argc, char *argv[])
{
  glfwSetErrorCallback(glfw_error_callback);
  
  if (!glfwInit())
    return 1;
  
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  
  GLFWwindow* window = glfwCreateWindow(1440, 800, "Demos", NULL, NULL);
  if (window == NULL)
    return 1;
  glfwMakeContextCurrent(window);
  
  bool err = glewInit() != GLEW_OK;
  
  if (err)
    {
      fprintf(stderr, "Failed to initialize OpenGL loader!\n");
      return 1;
    }
  
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = &ImGui::GetIO(); 
  
  ImGui::StyleColorsClassic();
  
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  
  while (!glfwWindowShouldClose(window))
    {
      glfwPollEvents();
      
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      
      basics::render();
      
      ImGui::Render();
      
      int display_w, display_h;
      glfwMakeContextCurrent(window);
      glfwGetFramebufferSize(window, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);
      glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      
      glfwMakeContextCurrent(window);
      glfwSwapBuffers(window);
      
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  
  
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  
  glfwDestroyWindow(window);
  glfwTerminate();
  
  return 0;
}
