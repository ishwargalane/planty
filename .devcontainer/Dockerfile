ARG VARIANT="ubuntu-22.04"
FROM mcr.microsoft.com/vscode/devcontainers/base:0-${VARIANT}

# Avoid warnings by switching to noninteractive
ENV DEBIAN_FRONTEND=noninteractive

# Set ESP-IDF version
ENV ESP_IDF_VERSION=release/v5.4
ENV ESP_IDF_REPO=https://github.com/espressif/esp-idf.git
ENV RMAKER_REPO=https://github.com/espressif/esp-rainmaker.git

# Install necessary packages
RUN apt-get update \
    && apt-get -y install --no-install-recommends \
    git wget flex bison gperf python3 python3-pip python3-venv python3-setuptools \
    cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0 \
    python3-dev libgirepository1.0-dev libcairo2-dev libpython3-dev pkg-config git \
    && apt-get clean -y && rm -rf /var/lib/apt/lists/*

# Create ESP user and directories
RUN useradd -m -s /bin/bash -G dialout esp \
    && mkdir -p /opt/esp \
    && chown -R esp:esp /opt/esp

# Switch to ESP user
USER esp

# Set up working directory
WORKDIR /opt/esp

# Clone ESP-IDF
RUN git clone --recursive --branch ${ESP_IDF_VERSION} ${ESP_IDF_REPO} idf

# Install ESP-IDF
RUN cd idf \
    && ./install.sh esp32 \
    && . ./export.sh

# Clone ESP-Rainmaker
RUN git clone ${RMAKER_REPO} rainmaker \
    && cd rainmaker \
    && git submodule update --init --recursive

# Prepare bash profile
RUN echo "source /opt/esp/idf/export.sh" >> ~/.bashrc \
    && echo "export RMAKER_PATH=/opt/esp/rainmaker" >> ~/.bashrc \
    && echo "alias get_idf='. $IDF_PATH/export.sh'" >> ~/.bashrc

# Switch back to root user
USER root

# Create symbolic links for executables
RUN ln -s /opt/esp/idf/tools/idf.py /usr/local/bin/idf.py

# Switch back to ESP user
USER esp

# Switch back to interactive
ENV DEBIAN_FRONTEND=dialog

WORKDIR /workspaces/planty 