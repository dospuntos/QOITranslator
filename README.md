# QOI image translator for Haiku

This is a Haiku image translator for QOI images, allowing them to be opened and saved in Haiku applications that support the Translation Kit.

QOI (Quite OK Image Format) is a fast, lossless image format designed as a simpler alternative to PNG.

QOI images for testing can be found at https://qoiformat.org/

## Installation

### Clone the repository

To get the source code, run:

```sh
git clone https://github.com/dospuntos/QOITranslator.git
cd QOITranslator
```

### Build the Translator

Compile the translator using `make`:

```sh
make
```

### Install the Translator

To install the translator in the Haiku system:

```sh
make install
```

This copies the `QOITranslator` binary to `~/config/non-packaged/add-ons/Translators`.

## Usage

Once installed, applications that use the Translation Kit (such as ShowImage) should automatically detect and open QOI images.

## Uninstallation

To remove the translator:

```sh
rm ~/config/non-packaged/add-ons/Translators/QOITranslator
```

## License

This project is licensed under the MIT License.
