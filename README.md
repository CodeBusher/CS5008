# CS5008 Lab

Welcome to the CS5008 lab repository!

We'll use this repository to share live coding examples, explore C programming, and practice working in a Linux x86-64 environment.

## 1. Getting Started

### Step 1: Fork This Repository

1. Sign in to [GitHub](https://github.com/).
2. Click **Fork** in the upper-right corner of this repository.
3. Keep the default settings and click **Create fork**.

You now have your own copy of the repository under your GitHub account.

### Step 2: Get Free Codespaces Hours with GitHub Education

GitHub provides a monthly free Codespaces allowance for personal accounts. Eligible students can also receive additional benefits through the **GitHub Student Developer Pack**.

1. Visit [GitHub Education](https://education.github.com/pack).
2. Sign in with your GitHub account.
3. Apply for the **GitHub Student Developer Pack** using your Northeastern student status.
4. Once approved, check that your GitHub Pro student benefit is active.

Verified students with GitHub Pro receive:

- **180 core-hours per month** (equivalent to 90 hours on a 2-core Codespace).
- **20 GB-month of Codespaces storage**.

GitHub Education approval may take time, but you can start using the free Codespaces allowance on a personal GitHub Free account while waiting.

**Important:** Codespaces usage is limited. Remember to stop your Codespace after each lab to conserve your free hours.

### Step 3: Create Your Codespace

1. Open **your forked repository**.
2. Click **Code → Codespaces → Create codespace on main**.
3. Wait for your development environment to start.

GitHub Codespaces provides a browser-based VS Code editor and Linux terminal. No local installation is required.

### Step 4: Verify Your Environment

Open the terminal and run:

```bash
uname -s
uname -m
gcc --version
```

For our labs, we expect:

- Operating system: `Linux`
- Architecture: `x86_64`
- C compiler: GCC

If `uname -m` reports something other than `x86_64`, please let the TA know.

## 2. Repository Structure

```text
cs5008-lab/
├── README.md       # Lab instructions
└── demo/           # TA's coding examples
```

The `demo/` folder contains examples demonstrated during lab sessions.

**You are welcome to create your own files and folders anywhere you prefer.**

Please avoid modifying `README.md` or files inside `demo/` on your `main` branch. This helps you receive TA updates without merge conflicts.

If you'd like to experiment with a TA example, you can make your own copy:

```bash
cp demo/pointers.c my_pointers.c
```

Then edit, compile, and run your own version:

```bash
gcc -Wall my_pointers.c -o my_pointers
./my_pointers
```

## 3. Get the Latest TA Updates

The TA will periodically update `README.md` and the `demo/` folder.

### One-Time Setup

In your Codespace terminal, run:

```bash
git remote -v
```

Your `origin` should point to your own fork.

Check whether the TA's repository is already configured as `upstream`. If it isn't, add it:

```bash
git remote add upstream https://github.com/CodeBusher/CS5008.git
```



You only need to configure `upstream` once per Codespace.

### During Each Lab

To download the TA's latest changes:

```bash
git fetch upstream
```

To incorporate those changes into your current branch:

```bash
git merge upstream/main
```

**Important:**

- `git fetch` downloads updates without changing your working files.
- `git merge` incorporates those updates into your current branch.
- Your own files will normally remain unchanged as long as they don't conflict with the TA's updates.
- If you've modified the same files as the TA, Git may report merge conflicts.

Before merging, check your working directory:

```bash
git status
```

If you have uncommitted changes, save or commit them before merging. If Git reports a conflict, ask the TA for help rather than discarding your work.

## 4. Save Your Work

You can save your work to your own GitHub fork:

```bash
git add .
git commit -m "commit message"
git push origin main
```

Your changes will be saved to **your own repository**, not the TA's repository.

You can also create additional branches to organize your experiments if you prefer.

## 5. Stop Your Codespace After Lab

Codespaces consumes compute resources while running.

When you're finished:

1. Visit [Your Codespaces](https://github.com/codespaces).
2. Find your Codespace.
3. Click the **⋯** menu and select **Stop codespace**.

You can restart your Codespace for the next lab without losing your saved workspace.

Stopping a Codespace stops compute usage, although storage usage may continue.

## Need Help?

If you run into issues with Git, Codespaces, GCC, or the lab exercises, feel free to ask during the session.

Happy coding!