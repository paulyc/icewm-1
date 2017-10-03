#ifndef __YMENU_H
#define __YMENU_H

#include "ypopup.h"
#include "ytimer.h"
#include "yaction.h"

class YActionListener;
class YMenuItem;

class YMenu: public YPopupWindow, public YTimerListener {
public:
    YMenu(YWindow *parent = 0);
    virtual ~YMenu();

    virtual void sizePopup(int hspace);
    virtual void activatePopup(int flags);
    virtual void deactivatePopup();
    virtual void donePopup(YPopupWindow *popup);

    virtual void paint(Graphics &g, const YRect &r);

    virtual bool handleKey(const XKeyEvent &key);
    virtual void handleButton(const XButtonEvent &button);
    virtual void handleMotion(const XMotionEvent &motion);
    virtual void handleMotionOutside(bool top, const XMotionEvent &motion);
    virtual bool handleAutoScroll(const XMotionEvent &mouse);

    void trackMotion(const int x_root, const int y_root, const unsigned state, bool submenu);

#ifndef LITE
    YMenuItem *add(YMenuItem *item, const char *icons);
    YMenuItem *addItem(const ustring &name, int hotCharPos, const ustring &param, tActionId action, const char *icons);
    YMenuItem *addItem(const ustring &name, int hotCharPos, tActionId action, YMenu *submenu, const char *icons);
    YMenuItem *addSubmenu(const ustring &name, int hotCharPos, YMenu *submenu, const char *icons);
#endif

    YMenuItem *add(YMenuItem *item);
    YMenuItem *addSorted(YMenuItem *item, bool duplicates, bool ignoreCase = false);
    YMenuItem *addItem(const ustring &name, int hotCharPos, const ustring &param, tActionId action);
    YMenuItem *addItem(const ustring &name, int hotCharPos, tActionId action, YMenu *submenu);
    YMenuItem *addSubmenu(const ustring &name, int hotCharPos, YMenu *submenu);
    YMenuItem *addSeparator();
    YMenuItem *addLabel(const ustring &name);
    void removeAll();
    YMenuItem *findAction(const tActionId action);
    YMenuItem *findSubmenu(const YMenu *sub);
    YMenuItem *findName(const ustring &name, const int first = 0);
    int findFirstLetRef(char firstLet, const int first, const int ignCase = 1);

    void enableCommand(tActionId action); // 0 == All
    void disableCommand(tActionId action); // 0 == All

    int itemCount() const { return fItems.getCount(); }
    YMenuItem *getItem(int n) const { return fItems[n]; }
    void setItem(int n, YMenuItem *ref) { fItems[n] = ref; return; }

    bool isShared() const { return fShared; }
    void setShared(bool shared) { fShared = shared; }

    void setActionListener(YActionListener *actionListener);
    YActionListener *getActionListener() const { return fActionListener; }

    virtual bool handleTimer(YTimer *timer);

private:
    YObjectArray<YMenuItem> fItems;
    int selectedItem;
    int paintedItem;
    int paramPos;
    int namePos;
    YPopupWindow *fPopup;
    YPopupWindow *fPopupActive;
    bool fShared;
    YActionListener *fActionListener;
    int activatedX, activatedY;
    int submenuItem;

#ifdef CONFIG_GRADIENTS
    ref<YImage> fGradient;
#endif

    static YMenu *fPointedMenu;
    static YTimer *fMenuTimer;
    int fTimerX, fTimerY;
    int fTimerSubmenuItem;
    static int fAutoScrollDeltaX, fAutoScrollDeltaY;
    static int fAutoScrollMouseX, fAutoScrollMouseY;

    void getOffsets(int &left, int &top, int &right, int &bottom);
    void getArea(int &x, int &y, int &w, int &h);

    void drawBackground(Graphics &g, int x, int y, int w, int h);
    void drawSeparator(Graphics &g, int x, int y, int w);

    void drawSubmenuArrow(Graphics &g, YMenuItem *mitem,
                          int left, int top);
    void paintItem(Graphics &g, const int i, const int l, const int t, const int r,
                   const int minY, const int maxY, bool draw);

    void repaintItem(int item);
    void paintItems();
    int findItemPos(int item, int &x, int &y, int &h);
    int findItem(int x, int y);
    int findActiveItem(int cur, int direction);
    int findHotItem(char k);
    void focusItem(int item);
    void activateSubMenu(int item, bool byMouse);

    int activateItem(int modifiers, bool byMouse = false);
    bool isCondCascade(int selectedItem);
    int onCascadeButton(int selectedItem, int x, int y, bool checkPopup);

    void autoScroll(int deltaX, int deltaY, int mx, int my, const XMotionEvent *motion);
    void finishPopup(YMenuItem *item, tActionId action, unsigned int modifiers);
    void hideSubmenu();
};

extern ref<YPixmap> menubackPixmap;
extern ref<YPixmap> menuselPixmap;
extern ref<YPixmap> menusepPixmap;

#ifdef CONFIG_GRADIENTS
//class YPixbuf;

extern ref<YImage> menubackPixbuf;
extern ref<YImage> menuselPixbuf;
extern ref<YImage> menusepPixbuf;
#endif

#endif
