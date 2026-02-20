#include "core/ProjectManager.h"
#include "core/RecentProjectsManager.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>

ProjectManager* ProjectManager::s_instance = nullptr;

ProjectManager::ProjectManager()
    : QObject(nullptr)
{
}

ProjectManager* ProjectManager::instance() {
    if (!s_instance) {
        s_instance = new ProjectManager();
    }
    return s_instance;
}

Project* ProjectManager::currentProject() const {
    return m_currentProject;
}

bool ProjectManager::hasOpenProject() const {
    return m_currentProject != nullptr && m_currentProject->isValid();
}

bool ProjectManager::createProject(const QString& name,
                                   const QString& location,
                                   const ProjectSettings& settings) {
    QString projectDir = location;
    if (settings.createProjectFolder) {
        projectDir = location + QDir::separator() + name;
    }
    
    QDir dir(projectDir);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            return false;
        }
    }
    
    // Create subdirectories
    dir.mkpath("src");
    dir.mkpath("include");
    dir.mkpath("build");
    
    // Create .gitignore
    QFile gitignore(projectDir + QDir::separator() + ".gitignore");
    if (gitignore.open(QIODevice::WriteOnly | QIODevice::Text)) {
        gitignore.write("build/\n*.o\n*.exe\n*.obj\n*.out\n");
        gitignore.close();
    }
    
    // Create main.cpp if requested
    if (settings.createMainCpp) {
        QFile mainFile(projectDir + QDir::separator() + "src" + QDir::separator() + "main.cpp");
        if (mainFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            mainFile.write(
                "#include <iostream>\n"
                "\n"
                "int main() {\n"
                "    std::cout << \"Hello, CppAtlas!\" << std::endl;\n"
                "    return 0;\n"
                "}\n"
            );
            mainFile.close();
        }
    }
    
    // Create CMakeLists.txt if requested
    if (settings.createCMakeLists) {
        QFile cmakeFile(projectDir + QDir::separator() + "CMakeLists.txt");
        if (cmakeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QString cmakeContent = QString(
                "cmake_minimum_required(VERSION 3.16)\n"
                "project(%1 LANGUAGES CXX)\n"
                "\n"
                "set(CMAKE_CXX_STANDARD 17)\n"
                "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n"
                "\n"
                "add_executable(%1 src/main.cpp)\n"
            ).arg(name);
            cmakeFile.write(cmakeContent.toUtf8());
            cmakeFile.close();
        }
    }
    
    // Create .cppatlas project file
    if (m_currentProject) {
        delete m_currentProject;
    }
    m_currentProject = new Project(this);
    m_currentProject->setName(name);
    m_currentProject->setDescription("");
    m_currentProject->setDirectory(projectDir);
    m_currentProject->setCompilerId(settings.compilerId);
    m_currentProject->setStandard(settings.standard);
    m_currentProject->setCompilerFlags(settings.compilerFlags);
    
    if (settings.createMainCpp) {
        m_currentProject->addSourceFile("src/main.cpp");
    }
    
    QString projectFilePath = projectDir + QDir::separator() + name + Project::FILE_EXTENSION;
    m_lastProjectFilePath = projectFilePath;
    
    if (!m_currentProject->save(projectFilePath)) {
        delete m_currentProject;
        m_currentProject = nullptr;
        return false;
    }
    
    RecentProjectsManager::instance()->addRecentProject(projectFilePath);
    
    emit projectCreated(m_currentProject);
    return true;
}

Project::LoadResult ProjectManager::openProject(const QString& filePath) {
    if (m_currentProject) {
        closeCurrentProject();
    }
    
    m_currentProject = new Project(this);
    Project::LoadResult result = m_currentProject->load(filePath);
    
    if (result != Project::LoadResult::Success) {
        delete m_currentProject;
        m_currentProject = nullptr;
        return result;
    }
    
    m_lastProjectFilePath = filePath;
    RecentProjectsManager::instance()->addRecentProject(filePath);
    
    emit projectOpened(m_currentProject);
    return Project::LoadResult::Success;
}

bool ProjectManager::saveCurrentProject() {
    if (!m_currentProject) {
        return false;
    }
    return m_currentProject->save();
}

bool ProjectManager::closeCurrentProject() {
    if (!m_currentProject) {
        return true;
    }
    
    if (m_currentProject->isModified()) {
        m_currentProject->save();
    }
    
    delete m_currentProject;
    m_currentProject = nullptr;
    
    emit projectClosed();
    return true;
}

QStringList ProjectManager::recentProjects() const {
    return RecentProjectsManager::instance()->recentProjects();
}

void ProjectManager::addToRecentProjects(const QString& path) {
    RecentProjectsManager::instance()->addRecentProject(path);
    emit recentProjectsChanged();
}

void ProjectManager::clearRecentProjects() {
    RecentProjectsManager::instance()->clearRecent();
    emit recentProjectsChanged();
}
