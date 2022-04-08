/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sdlDisplay
*/

#include <stdexcept>
#include <filesystem>
#include "sdlDisplay.hpp"
#include "sdlRawTexture.hpp"

sdl::SDLDisplay::SDLDisplay()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("Could not init SDL");
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
        throw std::runtime_error("Could not init IMG module");
    if (TTF_Init() == -1)
        throw std::runtime_error("Could not init TTF module");
    SDL_GetKeyboardState(&_numkeys);
    _oldkeys = new uint8_t[_numkeys];
    for (int i = 0; i < _numkeys; i++)
        _oldkeys[i] = false;
}

sdl::SDLDisplay::~SDLDisplay()
{
    for (auto &pair : _fonts)
        TTF_CloseFont(pair.second);
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void sdl::SDLDisplay::openWindow(Vector2u size)
{
    _window = SDL_CreateWindow("Arcade", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.x, size.y, SDL_WINDOW_RESIZABLE);
    if (_window == nullptr)
        throw std::runtime_error("Could not create window");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (_renderer == nullptr)
        throw std::runtime_error("Could not create renderer");
    SDL_RenderSetLogicalSize(_renderer, size.x, size.y);
}

void sdl::SDLDisplay::update()
{
    const uint8_t *keys = SDL_GetKeyboardState(NULL);
    SDL_Event event;

    for (int i = 0; i < _numkeys; i++)
        _oldkeys[i] = keys[i];
    _mouseButton = 0;
    _textInput.clear();
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                _close = true;
                break;
            case SDL_TEXTINPUT:
                _textInput += std::string(event.text.text);
                break;
            case SDL_MOUSEBUTTONDOWN:
                _mouseButton = event.button.button;
                _mousePos = {(uint) event.button.x, (uint) event.button.y};
                break;
        }
    }
}

bool sdl::SDLDisplay::isButtonPressed(IDisplayModule::Button button)
{
    const uint8_t *keys = SDL_GetKeyboardState(NULL);

    return !_oldkeys[SDL_KEYS.at(button)] && keys[SDL_KEYS.at(button)];
}

IDisplayModule::MouseButtonReleaseEvent sdl::SDLDisplay::getMouseButtonReleaseEvent()
{
    auto button = IDisplayModule::MouseButtonReleaseEvent{};

    switch (_mouseButton) {
        case SDL_BUTTON_LEFT:
            button.type = MouseButtonReleaseEvent::Type::Left;
            break;
        case SDL_BUTTON_RIGHT:
            button.type = MouseButtonReleaseEvent::Type::Right;
            break;
        default:
            button.type = MouseButtonReleaseEvent::Type::None;
            break;
    }
    button.cellPosition = {_mousePos.x / _pixelsPerCell, _mousePos.y / _pixelsPerCell};
    return button;
}

void sdl::SDLDisplay::startTextInput()
{
    SDL_StartTextInput();
}

std::string sdl::SDLDisplay::getTextInput()
{
    const uint8_t *keys;

    if (_textInput.empty()) {
        keys = SDL_GetKeyboardState(NULL);
        if (!_oldkeys[SDL_SCANCODE_BACKSPACE] && keys[SDL_SCANCODE_BACKSPACE])
            return "\b";
    }
    return _textInput;
}

void sdl::SDLDisplay::endTextInput()
{
    SDL_StopTextInput();
}

std::unique_ptr<IDisplayModule::RawTexture> sdl::SDLDisplay::loadTexture(const std::string &filename, char character, Color characterColor, Color backgroundColor, std::size_t width, std::size_t height)
{
    std::filesystem::path filenamePath{filename};

    (void) height;
    if (filenamePath.extension() == ".png")
        return std::make_unique<SDLRawGraphicTexture>(filename, Vector2u{(uint) width, (uint) height}, _renderer);
    if (filenamePath.extension() == ".ttf")
        return std::make_unique<SDLRawASCIITexture>(character, characterColor, backgroundColor, width, this->getFont(filename, width), _renderer);
    throw std::runtime_error("Tried to load texture from invalid file !");
}

void sdl::SDLDisplay::clearScreen(Color color)
{
    SDL_Color sdlColor = SDL_COLORS.at(color);

    SDL_SetRenderDrawColor(_renderer, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
    SDL_RenderClear(_renderer);
}

void sdl::SDLDisplay::renderSprite(IDisplayModule::Sprite sprite)
{
    sdl::ASDLRawTexture *texture = dynamic_cast<ASDLRawTexture*>(sprite.texture);
    SDL_Texture *sdlTexture = texture->getTexture();
    Vector2u size = texture->getSize();
    SDL_Rect dest = {(int) sprite.rawPixelPosition.x, (int) sprite.rawPixelPosition.y, (int) size.x, (int) size.y};

    SDL_RenderCopy(_renderer, sdlTexture, NULL, &dest);
}

void sdl::SDLDisplay::display()
{
    SDL_RenderPresent(_renderer);
}

TTF_Font *sdl::SDLDisplay::getFont(const std::string &filepath, std::size_t size)
{
    TTF_Font *font;
    auto findResult = _fonts.find({filepath, size});

    if (findResult != _fonts.end())
        return findResult->second;
    font = TTF_OpenFont(filepath.c_str(), size);
    if (font == nullptr)
        throw std::runtime_error("Could not load font " + filepath);
    _fonts.insert({{filepath, size}, font});
    return font;
}
