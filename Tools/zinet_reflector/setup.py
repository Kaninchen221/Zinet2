import setuptools


setuptools.setup(
    name="reflector",
    version="0.0.1",
    author="Pawel Krolik",
    author_email="",
    description="Generate reflection code (C++)",
    long_description="",
    long_description_content_type="",
    url="",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3"
    ],
    python_requires='>=3.6',
    install_requires=['numpy', 'pytest', 'pytest-mock', 'python-dateutil', 'clang'],
)
