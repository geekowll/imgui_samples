#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <thread>
#include <deque>
#include <vector>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace std;
using namespace ImGui;

class MouseHandler{
    enum class Button{
        LEFT_BUTTON = 0,
        RIGHT_BUTTON = 1,
        MIDDLE_BUTTON = 2
    };
    
    class MouseListener{
    public:
        virtual void onMouseClick(Button) = 0;
        virtual void onMouseRelease(Button) = 0;
        virtual void onMouseDragging(Button) = 0;
    };
    
    using mouse_listener_t = std::shared_ptr<MouseListener>;
    std::deque<mouse_listener_t> mListeners;
    
    void render(){
        auto clickControl = [=]{
            if(ImGui::IsMouseClicked(0))
                for(const auto &listener: mListeners)
                    listener->onMouseClick(Button::LEFT_BUTTON);
            
            if(ImGui::IsMouseClicked(1))
                for(const auto &listener: mListeners)
                    listener->onMouseClick(Button::RIGHT_BUTTON);
            
            if(ImGui::IsMouseClicked(2))
                for(const auto &listener: mListeners)
                    listener->onMouseClick(Button::MIDDLE_BUTTON);
        };
        
        auto releaseControl = [=]{
            if(ImGui::IsMouseReleased(0))
                for(const auto &listener: mListeners)
                    listener->onMouseRelease(Button::LEFT_BUTTON);
            
            if(ImGui::IsMouseReleased(1))
                for(const auto &listener: mListeners)
                    listener->onMouseRelease(Button::RIGHT_BUTTON);
            
            if(ImGui::IsMouseReleased(2))
                for(const auto &listener: mListeners)
                    listener->onMouseRelease(Button::MIDDLE_BUTTON);
        };
        
        auto draggingControl = [=]{
            if(ImGui::IsMouseDragging(0))
                for(const auto &listener: mListeners)
                    listener->onMouseDragging(Button::LEFT_BUTTON);
            
            if(ImGui::IsMouseDragging(1))
                for(const auto &listener: mListeners)
                    listener->onMouseDragging(Button::RIGHT_BUTTON);
            
            if(ImGui::IsMouseDragging(2))
                for(const auto &listener: mListeners)
                    listener->onMouseDragging(Button::MIDDLE_BUTTON);
        };
        
        clickControl();
        releaseControl();
        draggingControl();
    }
};

class KeyboardHandler{
    enum class Key{
        Tab,
        LeftArrow,
        RightArrow,
        UpArrow,
        DownArrow,
        PageUp,
        PageDown,
        Home,
        End,
        Insert,
        Delete,
        Backspace,
        Space,
        Enter,
        Escape,
        CTRL_A,         // for text edit CTRL+A: select all
        CTRL_C,         // for text edit CTRL+C: copy
        CTRL_V,         // for text edit CTRL+V: paste
        CTRL_X,         // for text edit CTRL+X: cut
        CTRL_Y,         // for text edit CTRL+Y: redo
        CTRL_Z,     
    };
    
    class KeyboardListener{
    public:
        virtual void onKeyPressed(int key) = 0;
        virtual void onKeyRelease(int key) = 0;
        virtual void onKeyDown(int key) = 0;
        virtual void onSpecialPressed(int key) = 0;
    };
    
    void render(){
        auto downControl = [=]{
            for(auto i = 0; i < KEYSSIZE; ++i){
                if(ImGui::IsKeyDown(i)){
                    for(const auto &listener : mListeners){
                        listener->onKeyDown(i);
                    }
                }
            }
        };
        auto pressControl = [=]{
            for(auto i = 0; i < KEYSSIZE; ++i){
                if(ImGui::IsKeyPressed(i)){
                    for(const auto &listener : mListeners){
                        listener->onKeyPressed(i);
                    }
                }
            }
        };
        auto releaseControl = [=]{
            for(auto i = 0; i < KEYSSIZE; ++i){
                if(ImGui::IsKeyReleased(i)){
                    for(const auto &listener : mListeners){
                        listener->onKeyRelease(i);
                    }
                }
            }
        };
        
        auto specialControl = [=]{
            for(auto i = (int)ImGuiKey_Tab; i < ImGuiKey_COUNT; ++i){
                if(ImGui::IsKeyPressed(i)){
                    for(const auto &listener : mListeners){
                        listener->onKeyRelease(i);
                    }
                }
            }
        };
        
        downControl();
        pressControl();
        releaseControl();
        specialControl();
    }
    
    constexpr static int KEYSSIZE = IM_ARRAYSIZE(ImGui::GetIO().KeysDown);
    
    using key_listener_t = std::shared_ptr<KeyboardListener>;
    std::deque<key_listener_t> mListeners;
};

namespace imgui_gl_sample{

bool operator==(ImVec2 left, ImVec2 right){
    return left.x == right.x && 
            left.y == right.y;
}

bool operator != (ImVec2 left, ImVec2 right){
    return !(left == right);
}

ostream& operator<<(ostream& os, ImVec2 vec){
    os << "{" << vec.x << "," << vec.y << "}";
    return os;
}

struct Shape{
    float border = 4.0f;
    ImColor color= ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
    operator ImU32() { return static_cast<ImU32>(color); }
};

struct Rectangle:Shape{
    ImVec2 startPos = {0,0};
    ImVec2 endPos = {0,0};
    
    operator bool() {
        return endPos == ImVec2{0,0} ? false : true;
    }
    
    void render() const {
        ImGui::GetWindowDrawList()->AddRect(startPos, 
                                            endPos, 
                                            color, 
                                            0.0f, 
                                            ImDrawCornerFlags_All, border); 
    }
}rect;

struct Circle:Shape{
    ImVec2 center = {0,0};
    float radius = 0.0f;
    
    operator bool() {
        return center != ImVec2(0,0) && radius != 0.0f ? true : false;
    }
    
    void render() const{ 
        
        ImGui::GetWindowDrawList()->AddCircle(center, 
                                              radius, 
                                              color, 
                                              50,
                                              border); 
    }
    
    void operator+=(ImVec2 pos){
        radius = ImSqrt( ImLengthSqr(ImVec2(center.x - pos.x,center.y - pos.y)));
    }
    
} circle;

ImVec2 operator*(int value,const ImVec2& vec){
    return { vec.x * value, vec.y*value };
}

ImVec2 operator-(int value, const ImVec2& vec){
    return { value - vec.x, value - vec.y };
}

ImVec2 operator-(const ImVec2& vec, int value){
    return { vec.x - value, vec.y - value };
}

ImVec2 operator+(const ImVec2& vec, int value){
    return { vec.x + value, vec.y + value };
}

ImVec2 normalize(ImVec2 vector) {
    float term = 1.0f/ImSqrt(ImLengthSqr(vector));
    return ImVec2(vector.x*term , vector.y*term );
}


struct Arrow: Shape{
    ImVec2 startPos = {0,0};
    ImVec2 endPos = {0,0};
    
    void render() const{
        ImGui::GetWindowDrawList()->AddLine(startPos, 
                                            endPos,
                                            color, 
                                            border);
        
        ImVec2 v = normalize(endPos - startPos);
        ImVec2 vPerp1 = ImVec2(-v.y , v.x );
        ImVec2 vPerp2 = ImVec2(v.y , -v.x );
        
        ImVec2 v1 = normalize(v + vPerp1);
        ImVec2 v2 = normalize(v + vPerp2);

        ImVec2 a = endPos ;
        ImVec2 b = endPos - 30*v1 ;
        ImVec2 c = endPos - 30*v2 ;

        ImGui::GetWindowDrawList()->AddTriangleFilled(a,b,c,color);
    }
    
    operator bool() { 
        return  endPos !=  ImVec2{0,0} && startPos != ImVec2{0,0};
    }
} arrow;


std::deque<Arrow> arrowList;

static void draw_arrow(){
    
    if(ImGui::IsMouseClicked(0)){
        arrow.startPos = ImGui::GetMousePos();
    }
    
    if(ImGui::IsMouseDragging(0)){
        arrow.endPos = ImGui::GetMousePos();
    }
    
    if(ImGui::IsMouseReleased(0)){
        if(arrow){
            arrowList.push_back(std::move(arrow));
            arrow = Arrow{};
        }
    }
    
    if (ImGui::IsKeyReleased(ImGui::GetKeyIndex(ImGuiKey_Z))){
        if(!arrowList.empty())
            arrowList.pop_back();
    }
    
    if(arrow)
        arrow.render();
    
    if(!arrowList.empty()){
        for(const auto& a : arrowList){
            a.render();
        }
    }
}

struct Polygon: Shape{
    std::vector<ImVec2> points;
    bool isClosed = false;
    void push(ImVec2 point){ points.push_back(point); }
    
    void pop() { points.erase(points.begin() + points.size()); }
    
    void render() const {
        ImGui::GetWindowDrawList()->AddPolyline(points.data(),
                                                points.size(),
                                                color,
                                                isClosed,
                                                border);
    }
}polygon;

std::deque<Polygon> polygonList;

static void draw_polygon(){
    if(ImGui::IsMouseClicked(0)){
        polygon.push(ImGui::GetMousePos());
    }else if(ImGui::IsMouseClicked(1)){
        polygon.isClosed = true;
        polygonList.push_back(polygon);
        polygon = {};
    }
    
    if (ImGui::IsKeyReleased(ImGui::GetKeyIndex(ImGuiKey_Z))){
        if(!polygonList.empty())
            polygonList.pop_back();
    }
    
    
    polygon.render();
    
    if(!polygonList.empty()){
        for(const auto& p: polygonList)
            p.render();
    }
}

ostream& operator<<(ostream& os, Rectangle rect){
    os << "start:" << rect.startPos << "\t" << rect.endPos << std::endl;
    return os;
}

ostream& operator<<(ostream& os, Circle circle){
    os << "center:" << circle.center << "\t" << "radius:" << circle.radius << std::endl;
    return os;
}

std::deque<Circle> circleList;

static void draw_circle(){
    if(ImGui::IsMouseClicked(0)){
        circle.center = ImGui::GetMousePos();
    }
    
    if(circle)
        circle.render();
    
    if(ImGui::IsMouseDragging(0)){
        circle += ImGui::GetMousePos();
    }
    
    
    if(ImGui::IsMouseReleased(0)){
        if(circle){
            circleList.push_back(std::move(circle));
            circle = Circle{};
        }
    }
    
    if(!circleList.empty()){
        for(const auto& c : circleList){
            c.render();
        }
    }
    
    if (ImGui::IsKeyReleased(ImGui::GetKeyIndex(ImGuiKey_Z))){
        if(!circleList.empty())
            circleList.pop_back();
    }
}

std::deque<Rectangle> rectList;

static void draw_rectange(){
    if(ImGui::IsMouseClicked(0)){
        rect.startPos = ImGui::GetMousePos();
    }
    
    if(ImGui::IsMouseDragging(0)){
        rect.endPos = ImGui::GetMousePos();
    }
    
    if(ImGui::IsMouseReleased(0)){
        if(rect){
            rectList.push_back(std::move(rect));
            rect = Rectangle{};
        }
    }
    
    if (ImGui::IsKeyReleased(ImGui::GetKeyIndex(ImGuiKey_Z))){
        if(!rectList.empty())
            rectList.pop_back();
    }
    
    if(rect)
        rect.render();
    
    if(!rectList.empty()){
        for(const auto& r : rectList){
            r.render();
        }
    }
}

static void render(){
    ImGui::SetNextWindowSize({555,555},ImGuiCond_Always);
    if(ImGui::Begin("imgui_gl_samples",nullptr,ImGuiWindowFlags_NoMove))
        draw_polygon();
    
    ImGui::End();
}
}

namespace imgui_detail{

// types
static void imgui_types(){
    ImGuiWindow* guiWindow; // one window
    ImDrawVert* drawVert;   // single vertex
    ImDrawCmd* drawCmd; //draw command
    ImDrawListSharedData* drawSharedData; // shared data among multiple lists
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

// ImGuiKey_ : custom key system

// ImGuiNavInput_

// ImGuiConfigFlags_

// ImGuiCol_
}

namespace imgui_basics{

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
    ImGui::BeginChild("##label",ImVec2(200,200));
    ImGui::Text("Test");
    ImGui::Text("Test 2");
    ImGui::EndChild();
}

// Tooltip
static void draw_tooltip(){
    ImGui::BeginChild("##tooltips",ImVec2(200,100),false,ImGuiWindowFlags_AlwaysAutoResize);
    
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
    
    ImGui::EndChild();
}

// Mouse position
static void print_mouse_current_position(){
    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 screenPos = ImGui::GetCursorScreenPos();
    ImVec2 cursorPos = ImGui::GetCursorPos();
    ImVec2 ioPos = ImGui::GetIO().MousePos;
    
    // which one ?
    
    ImGui::BeginChild("##mousepos",ImVec2(400,400),false,ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("mouse pos: %f : %f",mousePos.x,mousePos.y);
    ImGui::Text("screenPos: %f : %f",screenPos.x,screenPos.y);
    ImGui::Text("cursorPos: %f : %f",cursorPos.x,cursorPos.y);
    ImGui::Text("ioPos: %f : %f",ioPos.x,ioPos.y);
    ImGui::EndChild();
    
    // ImGui::GetIO().Mouse = mousePos => mouse cursor bilgisi
    // screenPos = draw cursor bilgisi
    // cursorPos = ne olduğu anlaşılamadı. TODO: ne bu
}

// mouse down- which button
static void mouse_control(){
    ImGui::Text("Mouse down:");
    
    int size = IM_ARRAYSIZE(ImGui::GetIO().MouseDown);
    
    ImGui::Text("Down size: %d ", size);
    
    for (int i = 0; i < size; i++){ 
        if (ImGui::GetIO().MouseDownDuration[i] >= 0.0f)   {
            ImGui::SameLine();
            ImGui::Text("button_%d (%g secs)", i, ImGui::GetIO().MouseDownDuration[i]); 
        }
    }
    
    ImGui::Text("Mouse clicked");
    for (int i = 0; i < size; ++i) {
        if(ImGui::IsMouseClicked(i)){
            ImGui::SameLine();
            ImGui::Text("Mouse clicked: button_%d",i);
        }
    }
    
    // MOuse clicked position TODO
    
    // Mouse clicked time
    
    // Mouse clicked inside a window or outer ?
    
    
    ImGui::Text("Mouse double clicked");
    for(int i = 0; i < size; ++i){
        if(ImGui::IsMouseDoubleClicked(i)){
            ImGui::SameLine();
            ImGui::Text("Mouse button double clicked: button_%d", i);
        }
    }
    
    ImGui::Text("Mouse released");
    for(int i = 0; i < size; ++i){
        if(ImGui::IsMouseReleased(i)){
            ImGui::SameLine();
            ImGui::Text("Released button: button_%d", i);
        }
    }
    
    ImGui::Text("Mouse Wheel : %g",ImGui::GetIO().MouseWheel);
    
    ImGui::Text("Mouse delta: [%g : %g ]", ImGui::GetIO().MouseDelta.x,ImGui::GetIO().MouseDelta.y);
}

// Keyboard pressed button
static void print_keyboard_events(){
    int size = IM_ARRAYSIZE(ImGui::GetIO().KeysDown);
    
    ImGui::Text("Key down:");
    for(auto i = 0; i < size; ++i){
        if(ImGui::IsKeyDown(i)){
            ImGui::SameLine();
            ImGui::Text("%d",i);
            if(ImGui::GetIO().KeysDownDuration[i] >= 0.0f){
                ImGui::SameLine();
                ImGui::Text(" %g secs",ImGui::GetIO().KeysDownDuration[i]);
            }
        }
    }
    
    ImGui::Text("Key pressed");
    for(auto i = 0; i < size; ++i){
        if(ImGui::IsKeyPressed(i)){
            ImGui::SameLine();
            ImGui::Text(" button_%d",i);
        }
    }
    
    ImGui::Text("Key released");
    for(auto i = 0; i < size; ++i){
        if(ImGui::IsKeyReleased(i)){
            ImGui::SameLine();
            ImGui::Text(" button%d",i);
        }
    }
    
    
    // Special keys TODO
    
}

// BulletText

// Column,Row

// Combobox
static void draw_combos(){
    const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", 
                            "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", 
                            "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
    static const char* item_current = items[0];           
    
    if (ImGui::BeginCombo("combo 1", item_current)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (item_current == items[n]);
            if (ImGui::Selectable(items[n], is_selected))
                item_current = items[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
        }
        ImGui::EndCombo();
    }
    
    static int item_current_2 = 0;
    ImGui::Combo("combo 2 (one-liner)", &item_current_2, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");
}

// Button

// Toggle

// Checkbox

// Groupbox

// Slider

// Progressbar

// ImGui::GetIO().keymap

// DragFloat

// Image

// Imagebutton

// Collapsingheader

// selectable


static void render(){
    bool open = true;
    
    if(ImGui::Begin("##basics")){
        
        draw_combos();
        
        ImGui::End();
    }
}
}

namespace imgui_more{

// groupbox

// layout

// Toolbar

// Menubar

// tabbar

// colordialog

// radiobutton

// TreeView

// Listbox

// NavItem

// TreeNodeEx

// CollapsingHeader

// Grouping ( BeginGroup - EndGroup)

// ChannelsSplit ? 
}

namespace imgui_ops{
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

// Get keyboard focus to pos SetKeyboardFocusHere PushAllowKeyboardFocus

}

namespace imgui_window_sample{
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
    
    GLFWwindow* window = glfwCreateWindow(1280, 960, "Demos", NULL, NULL);
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
        
        imgui_gl_sample::render();
        
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
