pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
    }
}

rootProject.name = "MyNDkDemo"
include(":app")
include(":day01")
include(":day02")
include(":ffmpeg-cmd")
include(":day03")
include(":day04")
include(":day05")
include(":day06")
include(":day07")
include(":day08")
