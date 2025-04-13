# Clockwork

**Clockwork** is a modular, extensible, and fully native **Hebrew calendar utility** for the [Haiku Operating System](https://www.haiku-os.org/), written in modern C++. It provides a rich set of tools for exploring the Hebrew calendar, Jewish holidays, lunar phases, weekly Torah readings, and related time-based rituals.

While its primary goal is to be a powerful date conversion and Jewish calendar reference, it also subtly introduces Haiku users to the structure, rhythm, and richness of Jewish timekeeping.

---

## ✨ Features

- 📅 Display today's Hebrew and Gregorian date, Torah portion, and candle lighting time
- 🔁 Bidirectional date conversion between Hebrew and Gregorian calendars
- 🌓 Graphical lunar phase display based on the Hebrew date
- 🔢 Days between dates (cross-calendar support)
- ⏳ Countdown to next holiday or event (with custom filters)
- 🕯️ Candle lighting and Havdalah times based on geolocation
- 🕎 Virtual Hanukkiah with lighting simulation
- 📜 Weekly Parashot and Haftarah reading info
- 🔗 Optional links to Wikipedia for holidays and events
- 🧩 Highly modular drag-and-drop interface: enable, disable, rearrange modules at will
- 🧲 Replicant support: pin modules to the desktop as native Haiku views
- 🌐 Multilingual support: UI language can follow system or be overridden manually

---

## 📷 Screenshots

*(Coming soon — once the initial UI stabilizes)*

---

## 🔧 Installation

Clockwork will be available via **HaikuDepot** and **HaikuPorts** once released.

Until then, you can build it manually:

```bash
git clone https://github.com/YOUR_USERNAME/clockwork.git
cd clockwork
make  # or jam
```

Dependencies:
- Haiku (latest nightly recommended)
- `libhdate` (used internally for Hebrew calendar calculations)
- Optional: `data_to_rdef` and `Icon-O-Matic` for embedding vector icons

---

## 📁 Project Structure

```
clockwork/
├── src/
│   ├── core/           # Hebrew date engine, wrapping libhdate
│   ├── ui/             # Main window, module host, settings
│   ├── modules/        # Individual modules: parsha, moon, countdown...
│   └── main.cpp
├── icons/              # SVG and HVIF icons
├── docs/               # DESIGN.md and dev notes
├── localization/       # *.catkeys, templates, translations, possibly - LanguageManager
├── tests/              # Test cases (unit-style)
└── README.md
```

---

## 🌍 Language Support

Clockwork uses Haiku's `.catkeys` localization system.

- Interface language can follow the OS language, or be overridden manually.
- Both Hebrew and transliterated date formats are supported.
- Date components can be shown in Arabic numerals or native letters (e.g., ג׳ בטבת).

---

## ✅ Roadmap & TODO

### Core Modules

- [x] Today module (current Hebrew date, parsha, candle lighting)
- [x] Torah portion module with Haftarah and weekday readings
- [x] Moon phase module with Bézier-rendered arc visualization
- [ ] Cross-calendar date converter with 5×7 grid view
- [ ] Difference between two dates (any calendar pair)
- [ ] Countdown to next holiday (configurable filters, sources)
- [ ] Virtual Hanukkiah with optional prayers
- [ ] Time-based reminders (e.g., yom hazikaron, yahrzeits)
- [ ] "This day in Jewish history" and quote-of-the-day module

### UI/UX

- [x] Modular drag-and-drop layout
- [x] Replicant support per module
- [ ] Unified preferences system (per-module + global)
- [ ] User-selectable color themes (light/dark modes)
- [ ] High-contrast mode and accessibility settings

### Technical

- [x] HebrewDate wrapper around `libhdate`
- [ ] BMessage-based state persistence
- [ ] Basic unit tests for date conversion and modules
- [ ] CI/CD via GitHub Actions (optional)

---

## 📜 License

This project is licensed under the **MIT License** — see [`LICENSE`](LICENSE) for details.

---

## 🤝 Contributing

At this stage, the project is under solo development. Contributors are welcome once the API stabilizes. If you're a Haiku user with interest in calendars, Hebrew, or just well-designed software — keep an eye on this repo or reach out via Issues!

---

## 📫 Contact

For bugs, suggestions, or collaboration ideas — please open a GitHub issue or contact the maintainer via my GitHub profile.
