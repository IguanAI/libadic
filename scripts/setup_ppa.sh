#!/bin/bash
# Setup script for creating a PPA (Personal Package Archive) on Launchpad

set -e

echo "==================================================="
echo "   libadic PPA Setup Script"
echo "==================================================="
echo ""

# Configuration
PACKAGE_NAME="libadic"
PPA_NAME="libadic-stable"
LAUNCHPAD_ID="your-launchpad-id"  # Change this!

echo "This script will help you set up a PPA for libadic on Launchpad."
echo ""
echo "Prerequisites:"
echo "  1. Launchpad account (https://launchpad.net)"
echo "  2. GPG key registered with Launchpad"
echo "  3. SSH key registered with Launchpad"
echo "  4. dput package installed (sudo apt-get install dput)"
echo ""

read -p "Have you completed all prerequisites? (y/n): " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Please complete the prerequisites first."
    echo "See: https://help.launchpad.net/Packaging/PPA"
    exit 1
fi

echo ""
echo "Step 1: Update changelog for PPA"
echo "---------------------------------"

# Get current version
VERSION=$(head -n1 debian/changelog | sed 's/.*(\(.*\)).*/\1/')
DIST=$(lsb_release -cs)

echo "Current version: $VERSION"
echo "Target distribution: $DIST"

# Create PPA-specific changelog entry
cp debian/changelog debian/changelog.backup
cat > debian/changelog.ppa << EOF
${PACKAGE_NAME} (${VERSION}~${DIST}1) ${DIST}; urgency=medium

  * PPA build for ${DIST}

 -- $(git config user.name) <$(git config user.email)>  $(date -R)

EOF
cat debian/changelog.backup >> debian/changelog.ppa
mv debian/changelog.ppa debian/changelog

echo "Changelog updated for PPA."

echo ""
echo "Step 2: Build source package"
echo "----------------------------"

# Clean previous builds
rm -f ../${PACKAGE_NAME}*.tar.* ../${PACKAGE_NAME}*.dsc ../${PACKAGE_NAME}*.changes

# Build source package (signed)
echo "Building signed source package..."
dpkg-buildpackage -S -sa

echo ""
echo "Step 3: Create dput configuration"
echo "---------------------------------"

if [ ! -f ~/.dput.cf ]; then
    cat > ~/.dput.cf << EOF
[${PPA_NAME}]
fqdn = ppa.launchpad.net
method = ftp
incoming = ~${LAUNCHPAD_ID}/ubuntu/${PPA_NAME}/
login = anonymous
allow_unsigned_uploads = 0
EOF
    echo "Created ~/.dput.cf configuration"
else
    echo "~/.dput.cf already exists"
fi

echo ""
echo "Step 4: Upload to PPA"
echo "--------------------"

CHANGES_FILE="../${PACKAGE_NAME}_${VERSION}_source.changes"

if [ -f "$CHANGES_FILE" ]; then
    echo "Ready to upload: $CHANGES_FILE"
    echo ""
    echo "To upload to your PPA, run:"
    echo "  dput ppa:${LAUNCHPAD_ID}/${PPA_NAME} $CHANGES_FILE"
    echo ""
    echo "After uploading, your PPA will be available at:"
    echo "  https://launchpad.net/~${LAUNCHPAD_ID}/+archive/ubuntu/${PPA_NAME}"
    echo ""
    echo "Users can then install with:"
    echo "  sudo add-apt-repository ppa:${LAUNCHPAD_ID}/${PPA_NAME}"
    echo "  sudo apt-get update"
    echo "  sudo apt-get install libadic-dev"
else
    echo "Error: Changes file not found: $CHANGES_FILE"
    exit 1
fi

echo ""
echo "Step 5: Multi-distribution builds (optional)"
echo "-------------------------------------------"
echo ""
echo "To build for multiple Ubuntu versions, use this script:"
cat > build_for_all_distros.sh << 'SCRIPT'
#!/bin/bash
DISTROS="focal jammy noble"  # Ubuntu 20.04, 22.04, 24.04
for DIST in $DISTROS; do
    sed -i "1s/.*/${PACKAGE_NAME} (${VERSION}~${DIST}1) ${DIST}; urgency=medium/" debian/changelog
    dpkg-buildpackage -S -sa
    dput ppa:${LAUNCHPAD_ID}/${PPA_NAME} ../${PACKAGE_NAME}_${VERSION}~${DIST}1_source.changes
done
SCRIPT
chmod +x build_for_all_distros.sh

echo "Created build_for_all_distros.sh for multi-distro builds"
echo ""
echo "==================================================="
echo "PPA setup complete!"
echo "==================================================="

# Restore original changelog
mv debian/changelog.backup debian/changelog