# Original sourced from GitLab Omniubs: https://gitlab.com/gitlab-org/omnibus-gitlab/-/blob/master/docker/Dockerfile
FROM ubuntu:20.04

# apt-get update
# apt-get install -y curl openssh-server ca-certificates tzdata
# apt-get install -y postfix
# curl https://packages.gitlab.com/install/repositories/gitlab/gitlab-ee/script.deb.sh | bash
# EXTERNAL_URL="http://gitlab.example.com" apt-get install gitlab-ee

# RUN apt-get update && apt-get install -y apt-utils curl openssh-server ca-certificates tzdata

# RUN curl https://packages.gitlab.com/install/repositories/gitlab/gitlab-ee/script.deb.sh | bash

# RUN  EXTERNAL_URL=http://localhost apt-get install gitlab-ee

# EXPOSE 22 80 443

# CMD [ "gitlab-ctl", "reconfigure" ]

#FROM ubuntu:16.04
# MAINTAINER GitLab Inc. <support@gitlab.com>

SHELL ["/bin/sh", "-c"],

# Default to supporting utf-8
ENV LANG=C.UTF-8

# Install required packages
RUN apt-get update -q \
    && DEBIAN_FRONTEND=noninteractive apt-get install -yq --no-install-recommends \
      ca-certificates \
      openssh-server \
      wget \
      apt-transport-https \
      vim \
      tzdata \
      nano \
      less \
    && rm -rf /var/lib/apt/lists/* \
    && sed 's/session\s*required\s*pam_loginuid.so/session optional pam_loginuid.so/g' -i /etc/pam.d/sshd

# Remove MOTD
RUN rm -rf /etc/update-motd.d /etc/motd /etc/motd.dynamic
RUN ln -fs /dev/null /run/motd.dynamic

# Copy assets
COPY RELEASE /
COPY assets/ /assets/
RUN /assets/setup

# Allow to access embedded tools
ENV PATH /opt/gitlab/embedded/bin:/opt/gitlab/bin:/assets:$PATH

# Resolve error: TERM environment variable not set.
ENV TERM xterm

# Expose web & ssh
EXPOSE 443 80 22

# Define data volumes
VOLUME ["/etc/gitlab", "/var/opt/gitlab", "/var/log/gitlab"]

# Wrapper to handle signal, trigger runit and reconfigure GitLab
CMD ["/assets/wrapper"]

HEALTHCHECK --interval=60s --timeout=30s --retries=5 \
CMD /opt/gitlab/bin/gitlab-healthcheck --fail --max-time 10
