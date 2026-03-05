#!/bin/sh

sudo true
ROOT_DIR="$(cd "$(dirname "$0")" && cd .. && pwd)"

EXLIBS_DIR="$ROOT_DIR/exlibs"

install_command() {
    for command in "$@"; do
        if ! type "$command" > /dev/null 2>&1; then
            sudo apt update
            sudo apt install -y "$command"
        fi
    done
}

get_package_manager() {
    if type apt > /dev/null 2>&1; then
        echo "apt"
    elif type dnf > /dev/null 2>&1; then
        echo "dnf"
    elif type yum > /dev/null 2>&1; then
        echo "yum"
    elif type pacman > /dev/null 2>&1; then
        echo "pacman"
    elif type zypper > /dev/null 2>&1; then
        echo "zypper"
    else
        echo "unsupported"
    fi
}

install_pkg() {
    local pkg_mgr=$(get_package_manager)
    if [ "$pkg_mgr" = "unsupported" ]; then
        echo "Error: Unsupported package manager."
        return 1
    fi

    for pkg in "$@"; do
        case "$pkg_mgr" in
            apt)
                if ! dpkg -s "$pkg" > /dev/null 2>&1; then
                    sudo apt update
                    sudo apt install -y "$pkg"
                fi
                ;;
            dnf|yum)
                if ! rpm -q "$pkg" > /dev/null 2>&1; then
                    sudo "$pkg_mgr" install -y "$pkg"
                fi
                ;;
            pacman)
                if ! pacman -Qi "$pkg" > /dev/null 2>&1; then
                    sudo pacman -Sy --noconfirm "$pkg"
                fi
                ;;
            zypper)
                if ! rpm -q "$pkg" > /dev/null 2>&1; then
                    sudo zypper install -y "$pkg"
                fi
                ;;
        esac
    done
}

install_command git g++ make cmake curl tar

if [ ! -d "googletest-1.17.0" ]; then
    TMP_DIR="$EXLIBS_DIR/googletest-1.17.0"
    curl -fsSL "https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz" | tar -xz -C "$EXLIBS_DIR"
    cmake -S "$TMP_DIR" -B "$TMP_DIR/build"
    cmake --build "$TMP_DIR/build" --config Release
fi
