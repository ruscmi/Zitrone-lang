# WARNING⚠️
Hello, this project has been abandoned, further promotion will be in the repository https://github.com/ruscmi/LemonenLang
# Zitrone-lang 🍋
**Zitrone-lang** is a fast and lightweight interpreter written in pure C. It is the successor to the `lmnlang` project, redesigned with a focus on performance, the use of hash tables, and proper memory management.

<p align="center">
	<img src="assets/media/banner_1280x593.jpg" width="100%" alt="Zitrone Banner">
</p>

## About project
Switching from C++ to pure C allowed us to make the interpreter more responsive and portable.
*	**Hash Table:** Used for quick access to variables.
*	**Strong Logic:** Type support (Int, Double, String, Bool) via `union`.
*	**Dual Mode:** Works as an interactive console (REPL) or by running `.zit` files.

<p align="center">
	<img src="assets/media/logo_1280x1280.jpg" width="150" alt="Zitrone Logo">
</p>

## Run locally

1. **Clone repo:**
```bash
git clone https://github.com/ruscmi/Zitrone-lang.git && cd Zitrone-lang
```

2. **Build project**
```bash
chmod +x build.sh
./build.sh
```

## Using

### REPL Mode (Interactive)
```bash
cd build
./ZitroneLang-Interpreter
```

### Run file
```bash
cd build
./ZitroneLang-Interpreter --file path/to/file.zit
```

## License

[MIT](https://choosealicense.com/licenses/mit/)


## Authors

- [@ruscmi](https://github.com/ruscmi/)
- [@voncov](https://github.com/voncov/)
