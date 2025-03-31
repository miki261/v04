#include "nwpwin.h"

class ship : public vsite::nwp::window {
public:
    std::string class_name() override { return "STATIC"; }
};

class main_window : public vsite::nwp::window {
    ship s;
    POINT position;
    bool moving = false;
protected:
    void on_left_button_down(POINT p) override {
        if (!s) {
            s.create(*this, WS_CHILD | WS_VISIBLE | SS_CENTER, "X", 0, p.x, p.y, 20, 20);
        }
        SetWindowPos(s, 0, p.x, p.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        position = p;
    }
    void on_key_up(int vk) override {
        if (s && moving) {
            SetWindowLong(s, GWL_STYLE, GetWindowLong(s, GWL_STYLE) & ~WS_BORDER);
            SetWindowPos(s, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
            moving = false;
        }
    }
    void on_key_down(int vk) override {
        if (!s) return;

        RECT rect;
        GetClientRect(*this, &rect);

        int step = (GetKeyState(VK_CONTROL) < 0) ? 30 : 5;
        POINT new_position = position;

        switch (vk) {
        case VK_LEFT:  new_position.x = max(0, position.x - step); 
            break;
        case VK_RIGHT: new_position.x = min(rect.right - 20, position.x + step); 
            break;
        case VK_UP:    new_position.y = max(0, position.y - step); 
            break;
        case VK_DOWN:  new_position.y = min(rect.bottom - 20, position.y + step); 
            break;
        default: 
            return;
        }

        if (new_position.x != position.x || new_position.y != position.y) {
            position = new_position;
            SetWindowPos(s, 0, position.x, position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            if (!moving) {
                SetWindowLong(s, GWL_STYLE, GetWindowLong(s, GWL_STYLE) | WS_BORDER);
                SetWindowPos(s, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
                moving = true;
            }
        }
    }
    void on_destroy() override {
        ::PostQuitMessage(0);
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow) {
    vsite::nwp::application app;
    main_window w;
    w.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, "NWP 4");
    return app.run();
}
